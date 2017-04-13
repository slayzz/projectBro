#include "server.hpp"
#include <sstream>

const unsigned int Server::MAX_CONNECTS = 5;


Server::Server(int port) {
  int retCode;
  struct sockaddr_in serv_addr, cli_addr;

  if (retCode = sockInit() != 0) {
    std::cout << "Sock Init error" << std::endl;
    exit(retCode);
  }

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0) {
    std::cout << "Sock Create error" << std::endl;
    exit(EXIT_FAILURE);
  }

#ifndef _WIN32
  int enable = 1;
  if (retCode = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable,
                           sizeof(int)) < 0) {
    std::cout << retCode << std::endl;
    std::cout << "Sock OPT error" << std::endl;
    exit(EXIT_FAILURE);
  }
#endif

  memset(&serv_addr, 0, sizeof(sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(socketFd, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
    std::cout << "Sock Bind error" << std::endl;
    exit(EXIT_FAILURE);
  }

  listen(socketFd, Server::MAX_CONNECTS);
  acceptConnections();
  clearSocket();
}

void Server::closeSocket() { clearSocket(); }

void Server::acceptConnections() {
  struct sockaddr cli_addr;
  memset(&cli_addr, 0, sizeof(sockaddr));
  while (1) {
    memset(buffer, '\0', BUFFER_LENGTH);
    unsigned int clientLenght = sizeof(cli_addr);
    SOCKET clientSocket =
        accept(socketFd, (struct sockaddr*)&cli_addr, &clientLenght);
    if (clientSocket < 0) {
      std::cout << "Sock Accept error" << std::endl;
      exit(EXIT_FAILURE);
    }

    int numberOfReaded;
    numberOfReaded = read(clientSocket, buffer, BUFFER_LENGTH);
    if (numberOfReaded < 0) {
      std::cout << "Client Sock Read error" << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << buffer << std::endl;

    formResponse("<div>SHOL NA PIPKA</div><h1> SUPER DUPER</h1>", clientSocket);
  }
}

void Server::formResponse(char* html, SOCKET clientSocket) {
  std::stringstream ss;
  ss << "HTTP/1.1 200 Ok\r\n"
     << "Server: Best Server Ever\r\n"
     << "Content-Type: text/html\r\n"
     << "Connection: keep-alive\r\n"
     << "\r\n\r\n"
     << html;

  std::string answer = ss.str();
  size_t length = answer.length();
  write(clientSocket, answer.c_str(), length);
  close(clientSocket);
  ;
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
  if (retCode = sockShutdown() != 0) {
    std::cout << "Sock Shutdown error" << std::endl;
    exit(retCode);
  }
  if (retCode = sockClose() != 0) {
    std::cout << "Sock Close error" << std::endl;
    exit(retCode);
  }
  close(socketFd);
}

Server::~Server() {}
