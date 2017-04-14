#include <iostream>

#include "RequestHandler.hxx"

SimpleHandler::SimpleHandler() : RequestHandler() {}
SimpleHandler::~SimpleHandler() {}


std::string SimpleHandler::getUrl() const {
  return "/some";
}

void SimpleHandler::handle(struct evhttp_request *) {
  std::cout << "SUPER LOL" << std::endl;
}


