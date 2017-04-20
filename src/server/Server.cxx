#include <sstream>

#include <event2/event.h>
#include <event2/bufferevent.h>

#include "Server.hxx"
#include "handlers/MainHandler.hxx"

const unsigned int Server::MAX_CONNECTS = 5;

Server::Server(uint16_t port): port_(port) {
  int retCode;
  struct sockaddr_in serverAddress;

  retCode = sockInit() != 0;
  if (retCode) {
    std::cout << "Sock Init error" << std::endl;
    exit(retCode);
  }

  socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd_ < 0) {
    std::cout << "Sock Create error" << std::endl;
    exit(EXIT_FAILURE);
  }
  evutil_make_socket_nonblocking(socketFd_);
  httpEvent_ = new HttpEvent(socketFd_);

#ifndef _WIN32
  int enable = 1;
  setsockopt(socketFd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
#endif

  memset(&serverAddress, 0, sizeof(sockaddr_in));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (bind(socketFd_, (sockaddr*)&serverAddress, sizeof(serverAddress))) {
    std::cout << "Sock Bind error" << std::endl;
    exit(EXIT_FAILURE);
  }
}

Server::~Server() {
  clearSocket();
}

void Server::run() {
  std::cout << "Server run on " << port_ << std::endl;
  listen(socketFd_, Server::MAX_CONNECTS);
  installHandlers();
  httpEvent_->run();
}


int Server::sockInit() {
#ifdef _WIN32
  WSADATA wsa_data;
  return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
  return 0;
#endif
}

int Server::sockClose() {
#ifdef _WIN32
  return WSACleanup();
#else
  return 0;
#endif
}

int Server::sockShutdown() {
  int status = 0;

#ifdef _WIN32
  status = shutdown(sock, SD_BOTH);
  if (status == 0) {
    status = closesocket(sock);
  }
#else
//    status = shutdown(socketFd, SHUT_RDWR);
//    if (status == 0) { status = close(socketFd); }
#endif

  return status;
}

void Server::clearSocket() {
  int retCode;
  retCode = sockShutdown() != 0;
  if (retCode) {
    std::cout << "Sock Shutdown error" << std::endl;
    exit(retCode);
  }
  retCode = sockClose() != 0;
  if (retCode) {
    std::cout << "Sock Close error" << std::endl;
    exit(retCode);
  }
  close(socketFd_);
}

void Server::installHandlers() {
  httpEvent_->addHandler(new MainHandler());
}

