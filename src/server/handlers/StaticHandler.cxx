#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <fstream>
#include <cstring>
#include <evhttp.h>
#include <iostream>
#include <sstream>

#include "StaticHandler.hxx"
StaticHandler::StaticHandler(std::string path) {
  staticFolderPath_ = "../" + path;
}

StaticHandler::~StaticHandler() {

}

void StaticHandler::handle(struct evhttp_request *request) {
  std::stringstream ss;
  auto evb = evbuffer_new();
  const size_t bufferSize = 10024;
  const char serverName[] = "Ultra Server";
  char buffer[bufferSize];
  std::string uri(evhttp_request_get_uri(request));
  auto accept = std::string(evhttp_find_header(request->input_headers, "Accept"));
  std::string type;
  int flag = -1;
  for (auto ch : accept) {
    if (ch == ',') {
      break;
    }
    type.push_back(ch);
  }


  std::ifstream file(staticFolderPath_ + uri);
  int position = 0;
  if (!file.is_open()) {
    throw std::runtime_error("Cant open this file");
  }

  while(!file.eof()) {
    file.get(buffer[position++]);
  }
  file.close();
  buffer[position-1] = '\0';
//
  evbuffer_add(evb, buffer, strlen(buffer));

  std::string contentType = type + std::string("; charset=utf-8");
// Set HTTP headers
  evhttp_add_header(request->output_headers, "Server", serverName);
  evhttp_add_header(request->output_headers, "Content-Length",
                    std::to_string(strlen(buffer)).c_str());
  evhttp_add_header(request->output_headers, "Content-Type", contentType.c_str());
//  evhttp_add_header(request->output_headers, "Content-Security-Policy",
//                    "default-src 'self'");

// Send reply
  evhttp_send_reply(request, HTTP_OK, "OK", evb);

// Free memory
  evbuffer_free(evb);
}

std::string StaticHandler::getUrl() const {
  return staticFolderPath_;
}
