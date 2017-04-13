#ifndef MY_SERVER_HPP
#define MY_SERVER_HPP

#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
  #endif
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#else
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif

#include <iostream>
#include <string.h>
#include <event2/event.h>

typedef evutil_socket_t SOCKET;

const unsigned int BUFFER_LENGTH = 2048;

class Server {
public:
  explicit Server(int port);
  ~Server();

  void closeSocket();
  void run();
  struct event_base* getEventBase();
private:
  SOCKET socketFd_;
  char buffer_[BUFFER_LENGTH];

  static const unsigned int MAX_CONNECTS;
  struct event_base* eventBase_;

  int sockInit();
  int sockClose();
  int sockShutdown();
  void clearSocket();

  void formResponse(char*, SOCKET);
//  void acceptConnections(struct bufferevent *bev, void *ctx);

  void static readClientSocket(struct bufferevent *bev, void *ctx);
  void static errorHandlingClientSocket(struct bufferevent *bev, short error,
                                 void *ctx);
  void static doAccept(SOCKET listener, short event, void *arg);
};

#endif
