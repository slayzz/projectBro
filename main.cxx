#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <sys/wait.h>

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

//int main(int argc, char *argv[]) {
//  pid_t pid = fork();
//  if (pid == 0) {
////   child process
//    startServer();
//  } else if (pid > 0) {
////  parent process
//    startWebsocket();
//  } else {
//    // fork failed
//    std::cerr << "fork() failed!\n" << std::endl;
//    return 1;
//  }
//
//  return 0;
//}

static const char SERVER_SESSION[] = "/tmp/server_session.tmp";
static const char DAEMON_SESSION[] = "/tmp/daemon_session.tmp";

void test() {

  using namespace std;
  int fdServer, fdDaemon;
  int counter;

  fdServer = open(SERVER_SESSION, O_WRONLY);
  if (fdServer < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  char willBeWrited[] = "FromServer:zxcvasdfqwer";

  counter = write(fdServer, willBeWrited, strlen(willBeWrited));
  if (counter < 0) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  close(fdServer);
  fdDaemon = open(DAEMON_SESSION, O_RDONLY);
  if (fdDaemon < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  char buffer[1024];
  counter = read(fdDaemon, buffer, 1024);
  if (counter < 0) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  buffer[counter] = '\0';
  close(fdDaemon);
  std::cout << buffer << std::endl;
}


int main(int argc, char *argv[]) {
  test();

//  pid_t cpid;
//  cpid = fork();
//  if (cpid == 0) {
//    parentProcess(pipefd);
//  } else if (cpid > 0) {
//    childProcess(pipefd);
//  } else {
//    exit(EXIT_FAILURE);
//  }
}
