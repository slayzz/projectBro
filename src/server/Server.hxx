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

#include "HttpEvent.hxx"
#include <iostream>
#include <string.h>
#include <event2/event.h>
#include "../globals.hxx"


class Server {
public:
  Server(uint16_t port);
  ~Server();
  Server& operator=(const Server&) = delete;

  void run();
  void closeSocket();
private:
  HttpEvent* httpEvent_;
  Types::SOCKET socketFd_;
  uint16_t port_;

  static const unsigned int MAX_CONNECTS;

  int sockInit();
  int sockClose();
  int sockShutdown();
  void clearSocket();
  void installHandlers();
};

#endif
