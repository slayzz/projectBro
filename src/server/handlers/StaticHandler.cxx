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
  const auto accept = req.getHeader("Accept");
  const auto uri = req.getUri();
  const auto parsedUri = Common::split(uri, std::string("/"));
  const auto filePair = Common::getFilePair(parsedUri.back());
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

  req.setHeader("Content-Length", std::move(std::to_string(strlen(fillBuffer))));
  req.setHeader("Content-Type", std::move(contentType));


  req.writeBody(fillBuffer);
  req.end();
  delete[] fillBuffer;

}
