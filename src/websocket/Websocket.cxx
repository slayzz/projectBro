#include "Websocket.hxx"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

Websocket::Websocket() {
  websocketServer_.init_asio();
  websocketServer_.set_reuse_addr(true);

  websocketServer_.set_http_handler(bind(&Websocket::onHttp, this, _1));
  websocketServer_.set_open_handler(bind(&Websocket::onOpen, this, _1));
  websocketServer_.set_close_handler(bind(&Websocket::onClose, this, _1));
  websocketServer_.set_message_handler(bind(&Websocket::onMessage, this, _1, _2));
}

Websocket::~Websocket() { }

void Websocket::run(uint16_t port) {
  try {
    thread t(bind(&Websocket::processMessage, this));

    websocketServer_.listen(port);
    websocketServer_.start_accept();
    websocketServer_.run();

    t.join();
  } catch (const std::exception & e) {
    std::cout << e.what() << std::endl;
  }
}

void Websocket::onOpen(connection_hdl hdl) {
  {
    lock_guard<mutex> guard(mutexActionLock_);
    auto connection = websocketServer_.get_con_from_hdl(hdl);
    auto body = connection->get_request_body();
    auto key = connection->get_request_header("Sec-WebSocket-Key");
    std::cout << body << std::endl;
    std::cout << key << std::endl;
    mutexActions_.push(Action(SUBSCRIBE, hdl));
  }
  mutexActionCondition_.notify_one();
}

void Websocket::onClose(connection_hdl hdl) {
  {
    lock_guard<mutex> guard(mutexActionLock_);
    mutexActions_.push(Action(UNSUBSCRIBE, hdl));
  }
  mutexActionCondition_.notify_one();
}

void Websocket::onMessage(connection_hdl hdl, Types::MessagePtr msg) {
  {
    lock_guard<mutex> guard(mutexActionLock_);
    mutexActions_.push(Action(MESSAGE, hdl, msg));
  }
  mutexActionCondition_.notify_one();
}

void Websocket::onHttp(connection_hdl hdl) {
  {
    lock_guard<mutex> guard(mutexActionLock_);
    Types::WebsocketBaseServer::connection_ptr connection =
        websocketServer_.get_con_from_hdl(hdl);
    std::string str = connection->get_request_body();
    std::cout << "ULTRA RESPONSE -> " << str << std::endl;
  }
}

void Websocket::processMessage() {
  while(true) {
    unique_lock<mutex> lock(mutexActionLock_);

    while(mutexActions_.empty()) {
      mutexActionCondition_.wait(lock);
    }

    Action action = mutexActions_.front();
    mutexActions_.pop();

    lock.unlock();

    if (action.type == SUBSCRIBE) {
      lock_guard<mutex> guard(mutexConnectionLock_);
      mutexConnections_.insert(action.handler);
    } else if (action.type == UNSUBSCRIBE) {
      lock_guard<mutex> guard(mutexConnectionLock_);
      mutexConnections_.erase(action.handler);
    } else if (action.type == MESSAGE) {
      lock_guard<mutex> guard(mutexConnectionLock_);

      for (auto connection : mutexConnections_) {
        websocketServer_.send(connection, action.message->get_payload(), action.message->get_opcode());
      }
    }
  }

}
