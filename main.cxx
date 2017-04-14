#include <iostream>

#include <event2/event.h>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "src/server/server.hxx"

//typedef websocketpp::server<websocketpp::config::asio> server;

//using websocketpp::lib::placeholders::_1;
//using websocketpp::lib::placeholders::_2;
//using websocketpp::lib::bind;
//
//typedef server::message_ptr message_ptr;

//void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg) {
//  std::cout << "on_message called with hdl: " << hdl.lock().get()
//            << " and message: " << msg->get_payload() << std::endl;
//
//  if (msg->get_payload() == "stop-listening") {
//    s->stop_listening();
//    return;
//  }
//
//  try {
//    s->send(hdl, msg->get_payload(), msg->get_opcode());
//  } catch (const websocketpp::lib::error_code& e) {
//    std::cout << "Echo failed because: " << e
//              << "(" << e.message() << ")" << std::endl;
//  }
//}

int main(int argc, char *argv[]) {
  Server server(8000);
  server.run();
//  server echo_server;

//  try
//  {
//    echo_server.set_access_channels(websocketpp::log::alevel::all);
//    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

//    echo_server.init_asio();

//    echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));


//    echo_server.listen(9002);

//    echo_server.start_accept();

//    echo_server.run();

//  } catch (websocketpp::exception const& e)
//  {
//    std::cout << e.what() << std::endl;
//    return 100;
//  } catch (...)
//  {
//    std::cout << "other exception" << std::endl;
//    return 101;
//  }
}
