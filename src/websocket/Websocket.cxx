#include "Websocket.hxx"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


void websocketRunner() {
  Types::WebsocketBaseServer echoServer;

  try {
    echoServer.set_access_channels(websocketpp::log::alevel::all);
    echoServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

    echoServer.init_asio();

    echoServer.set_message_handler(bind(&on_message, &echoServer, ::_1, ::_2));

    echoServer.listen(9002);

    echoServer.start_accept();

    echoServer.run();

  } catch (websocketpp::exception const& e)
  {
    std::cout << e.what() << std::endl;
  } catch (...)
  {
    std::cout << "other exception" << std::endl;
  }
}

void on_message(Types::WebsocketBaseServer *s, websocketpp::connection_hdl hdl,
                Types::MessagePtr msg) {
  std::cout << "on_message called with hdl: " << hdl.lock().get()
            << " and message: " << msg->get_payload() << std::endl;

  if (msg->get_payload() == "stop-listening") {
    s->stop_listening();
    return;
  }

  try {
    s->send(hdl, msg->get_payload(), msg->get_opcode());
  } catch (const websocketpp::lib::error_code& e) {
    std::cout << "Echo failed because: " << e
              << "(" << e.message() << ")" << std::endl;
  }
}
Websocket::Websocket() {
  websocketServer.init_asio();

  websocketServer.set_open_handler(bind(&Websocket::onOpen, this, _1));
  websocketServer.set_close_handler(bind(&Websocket::onClose, this, _1));
  websocketServer.set_message_handler(bind(&Websocket::onMessage, this, _1, _2));
}

Websocket::~Websocket() { }

void Websocket::run(uint16_t port) {
  try {
    thread t(bind(&Websocket::processMessage, this));

    websocketServer.listen(port);
    websocketServer.start_accept();

    websocketServer.run();

    t.join();
  } catch (const std::exception & e) {
    std::cout << e.what() << std::endl;
  }
}
void Websocket::onOpen(connection_hdl hdl) {
  {
    lock_guard<mutex> guard(mutexActionLock);
    mutexActions.push(Action(SUBSCRIBE, hdl));
  }
  mutexActionCondition.notify_one();
}

void Websocket::onClose(connection_hdl hdl) {
  {
    lock_guard<mutex> guard(mutexActionLock);
    mutexActions.push(Action(UNSUBSCRIBE, hdl));
  }
  mutexActionCondition.notify_one();
}

void Websocket::onMessage(connection_hdl hdl, Types::MessagePtr msg) {
  {
    lock_guard<mutex> guard(mutexActionLock);
    mutexActions.push(Action(MESSAGE, hdl, msg));
  }
  mutexActionCondition.notify_one();
}

void Websocket::processMessage() {
  while(true) {
    unique_lock<mutex> lock(mutexActionLock);

    while(mutexActions.empty()) {
      mutexActionCondition.wait(lock);
    }

    Action action = mutexActions.front();
    mutexActions.pop();

    lock.unlock();

    if (action.type == SUBSCRIBE) {
      lock_guard<mutex> guard(mutexConnectionLock);
      mutexConnections.insert(action.handler);
    } else if (action.type == UNSUBSCRIBE) {
      lock_guard<mutex> guard(mutexConnectionLock);
      mutexConnections.erase(action.handler);
    } else if (action.type == MESSAGE) {
      lock_guard<mutex> guard(mutexConnectionLock);

      Types::ConnectionList::iterator it;
      for (auto connection : mutexConnections) {
        std::cout << action.message->get_payload() << std::endl;
        websocketServer.send(connection, action.message->get_payload(), action.message->get_opcode());
//        websocketServer.send(connection, action.message);
      }
    }
  }

}
