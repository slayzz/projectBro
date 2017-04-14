#include <sstream>

#include <event2/event.h>
#include <event2/bufferevent.h>

#include "server.hxx"

const unsigned int Server::MAX_CONNECTS = 5;

Server::Server(uint16_t port) {
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

void Server::run() {
  listen(socketFd_, Server::MAX_CONNECTS);
  httpEvent_->run();
}

//void Server::doAccept(SOCKET listener, short event, void *arg) {
//  Server* server = static_cast<Server*>(arg);
//  struct sockaddr_storage ss;
//
//  socklen_t slen = sizeof(ss);
//  int clientSocket = accept(listener, (struct sockaddr*)&ss, &slen);
//  if (clientSocket < 0) {
//    std::cerr << "Cant accept connection" << std::endl;
//    throw "Cant accept connection";
//  } else if (clientSocket > FD_SETSIZE) {
//    close(clientSocket);
//  } else {
//    struct bufferevent *bev;
//    evutil_make_socket_nonblocking(clientSocket);
//    bev = bufferevent_socket_new(server->getEventBase(), clientSocket,
//                                 BEV_OPT_CLOSE_ON_FREE);
//    bufferevent_setcb(bev, &Server::readClientSocket, NULL,
//                      &Server::errorHandlingClientSocket, NULL);
//    bufferevent_setwatermark(bev, EV_READ, 0, 10000);
//    bufferevent_enable(bev, EV_READ|EV_WRITE);
//  }
//}
//
//void Server::readClientSocket(struct bufferevent *bev, void *ctx) {
//  struct evbuffer *input, *output;
//  char *line;
//  size_t n;
//  input = bufferevent_get_input(bev);
//  output = bufferevent_get_output(bev);
//
//  while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {
//    std::cout << line << std::endl;
//    evbuffer_add(output, line, n);
//    evbuffer_add(output, "\n", 1);
//    free(line);
//  }
//
//  if (evbuffer_get_length(input) >= 10000) {
//    /* Too long; just process what there is and go on so that the buffer
//     * doesn't grow infinitely long. */
//    char buf[1024];
//    while (evbuffer_get_length(input)) {
//        int n = evbuffer_remove(input, buf, sizeof(buf));
//        evbuffer_add(output, buf, n);
//    }
//    evbuffer_add(output, "\n", 1);
//  }
//}
//
//void Server::errorHandlingClientSocket(struct bufferevent *bev, short error, void *ctx) {
//  if (error & BEV_EVENT_EOF) {
//    /* connection has been closed, do any clean up here */
//    /* ... */
//  } else if (error & BEV_EVENT_ERROR) {
//    /* check errno to see what error occurred */
//    /* ... */
//  } else if (error & BEV_EVENT_TIMEOUT) {
//    /* must be a timeout event handle, handle it */
//    /* ... */
//  }
//  bufferevent_free(bev);
//}

//void Server::formResponse(char* html, SOCKET clientSocket) {
//  std::stringstream ss;
//  ss << "HTTP/1.1 200 Ok\r\n"
//     << "Server: Best Server Ever\r\n"
//     << "Content-Type: text/html\r\n"
//     << "Connection: keep-alive\r\n"
//     << "\r\n\r\n"
//     << html;
//
//  std::string answer = ss.str();
//  size_t length = answer.length();
//  write(clientSocket, answer.c_str(), length);
//  close(clientSocket);
//  ;
//}

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
  if (retCode = sockShutdown() != 0) {
    std::cout << "Sock Shutdown error" << std::endl;
    exit(retCode);
  }
  if (retCode = sockClose() != 0) {
    std::cout << "Sock Close error" << std::endl;
    exit(retCode);
  }
  close(socketFd_);
}

Server::~Server() {
  clearSocket();
}
