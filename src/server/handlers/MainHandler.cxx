#include <iostream>
#include <fstream>
#include <cstring>

#include "MainHandler.hxx"

MainHandler::MainHandler(): RequestHandler() { }

MainHandler::~MainHandler() { }

std::string MainHandler::getUrl() const {
  return "/";
}

void MainHandler::handle(Request req) {
  const size_t bufferSize = 10024;
  char buffer[bufferSize];

  int position = 0;
  std::ifstream file("../web/html/index.html");
  while(!file.eof()) {
    file.get(buffer[position++]);
  }
  buffer[position-1] = '\0';

// Set HTTP headers
  req.setHeader("Connection", "close");
  req.setHeader("Content-Length", std::to_string(strlen(buffer)).c_str());
  req.setHeader("Content-Type", "text/html; charset=utf-8");

  req.writeBody(buffer);
  file.close();
  req.end();
}
