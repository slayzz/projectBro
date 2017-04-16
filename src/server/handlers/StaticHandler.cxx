#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <fstream>
#include <cstring>
#include <evhttp.h>
#include <iostream>
#include <sstream>
#include "../../globals.hxx"
#include "../common/common.hxx"

#include "StaticHandler.hxx"
StaticHandler::StaticHandler(std::string path) {
  staticFolderPath_ = "../" + path;
}

StaticHandler::~StaticHandler() { }


std::string StaticHandler::getUrl() const {
  return staticFolderPath_;
}

void StaticHandler::readFile(std::string uri, char* buffer) {
  std::ifstream file(staticFolderPath_ + uri);
  if (!file.is_open()) {
    throw std::runtime_error("Cant open this file");
  }

  int position = 0;
  while(!file.eof()) {
    file.get(buffer[position++]);
  }
  file.close();
  buffer[position-1] = '\0';
}

void StaticHandler::handle(Request req) {
  auto request = req.getRawRequest();
  auto evb = evbuffer_new();
  const auto accept = std::string(evhttp_find_header(request->input_headers, "Accept"));
  const auto uri = std::string(evhttp_request_get_uri(request));
  const auto parsedUri = Common::split(uri, std::string("/"));
  const Common::FilePair filePair = Common::getFilePair(parsedUri.back());
  std::string contentType;

  if (filePair.second == "js") {
    contentType = "application/javascript; charset=utf-8";
  } else if (filePair.second == "css") {
    contentType = "text/css; charset=utf-8";
  } else {
    contentType = "text; charset=utf-8";
  }

  char* fillBuffer = new char[10000];
  readFile(uri, fillBuffer);
  evbuffer_add(evb, fillBuffer, strlen(fillBuffer));

  evhttp_add_header(request->output_headers, "Server", Globals::SERVER_NAME);
  evhttp_add_header(request->output_headers, "Content-Length",
                    std::to_string(strlen(fillBuffer)).c_str());
  evhttp_add_header(request->output_headers, "Content-Type", contentType.c_str());

  evhttp_send_reply(request, HTTP_OK, "OK", evb);

  evbuffer_free(evb);

  delete[] fillBuffer;

}
