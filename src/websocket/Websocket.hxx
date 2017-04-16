#ifndef WEBSOCKET_WEBSOCKET_HXX
#define WEBSOCKET_WEBSOCKET_HXX

#include <queue>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include "../globals.hxx"

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

enum ActionType {
  SUBSCRIBE,
  UNSUBSCRIBE,
  MESSAGE
};

struct Action {
  Action(ActionType t, connection_hdl hdl)
      : type(t), handler(hdl) {}

  Action(ActionType t, connection_hdl hdl, Types::MessagePtr msg)
      : type(t), handler(hdl), message(msg) {}

  ActionType type;
  websocketpp::connection_hdl handler;
  Types::MessagePtr message;
};

class Websocket {

 public:
  explicit Websocket();
  ~Websocket();
  void run(uint16_t);

 private:
  Types::WebsocketBaseServer websocketServer_;
  Types::ConnectionList mutexConnections_;
  std::queue<Action> mutexActions_;

  mutex mutexActionLock_;
  mutex mutexConnectionLock_;
  condition_variable mutexActionCondition_;

  void onOpen(connection_hdl);
  void onClose(connection_hdl);
  void onMessage(connection_hdl, Types::MessagePtr);
  void onHttp(connection_hdl hdl);

  void processMessage();

};

#endif //WEBSOCKET_WEBSOCKET_HXX
