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

typedef unsigned int SOCKET;

const unsigned int BUFFER_LENGTH = 2048;

class Server {
public:
  Server(int port);
  ~Server();

  void closeSocket();
private:
  SOCKET socketFd;
  char buffer[BUFFER_LENGTH];

  static const unsigned int MAX_CONNECTS;


  void formResponse(char*, SOCKET);
  int sockInit();
  int sockClose();
  int sockShutdown();
  void acceptConnections();
  void clearSocket();
};

#endif
