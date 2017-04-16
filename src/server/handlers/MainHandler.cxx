#include <iostream>
#include <fstream>
#include <cstring>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

#include "MainHandler.hxx"

MainHandler::MainHandler(): RequestHandler() { }

MainHandler::~MainHandler() { }

std::string MainHandler::getUrl() const {
  return "/";
}

void MainHandler::handle(Request req) {
  auto request = req.getRawRequest();
  auto evb = evbuffer_new();
  auto headers = evhttp_request_get_input_headers(request);
  auto outputHeaders = evhttp_request_get_output_headers(request);

  const size_t bufferSize = 10024;
  const char serverName[] = "Ultra v.01";
  char buffer[bufferSize];

  int position = 0;
  std::ifstream file("../web/html/index.html");
  while(!file.eof()) {
    file.get(buffer[position++]);
  }
  buffer[position-1] = '\0';

  evbuffer_add(evb, buffer, strlen(buffer));

// Set HTTP headers
  evhttp_add_header(outputHeaders, "Server", serverName);
  evhttp_add_header(outputHeaders, "Connection", "close");
  evhttp_add_header(outputHeaders, "Content-Length",
                    std::to_string(strlen(buffer)).c_str());
  evhttp_add_header(outputHeaders, "Content-Type",
                    "text/html; charset=utf-8");

// Send reply
  evhttp_send_reply(request, HTTP_OK, "OK", evb);

  file.close();
// Free memory
  evbuffer_free(evb);
}
