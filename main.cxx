#include <iostream>
#include <cstring>

#include "src/server/Server.hxx"
#include "src/websocket/Websocket.hxx"

void startServer() {
  Server server(8000);
  server.run();
}

void startWebsocket() {
  Websocket websocket;
  websocket.run(9000);
}

using namespace std;

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  if (pid == 0) {
    // child process
    startWebsocket();
  }
  else if (pid > 0) {
    // parent process
    startServer();
  } else {
    // fork failed
    std::cerr << "fork() failed!\n" << std::endl;
    return 1;
  }
  return 0;
}
