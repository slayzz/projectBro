#ifndef WEBSOCKET_GLOBALS_HXX
#define WEBSOCKET_GLOBALS_HXX
#include <event2/event.h>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
using websocketpp::connection_hdl;


namespace Types {
  typedef evutil_socket_t SOCKET;

  // Websocket TypeDef
  typedef websocketpp::server<websocketpp::config::asio> WebsocketBaseServer;
  typedef WebsocketBaseServer::message_ptr MessagePtr;
  typedef std::set<connection_hdl,std::owner_less<connection_hdl> >
      ConnectionList;
}

namespace Globals {
  static char SERVER_NAME[] = "MegA ServeR";
}

#endif //WEBSOCKET_GLOBALS_HXX
