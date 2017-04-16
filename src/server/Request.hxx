#ifndef WEBSOCKET_REQUEST_HXX
#define WEBSOCKET_REQUEST_HXX

#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <string>
#include <map>

class Request final {
 public:
  typedef std::map<std::string, std::string> HeaderMap;
  typedef std::pair<std::string, std::string> HeaderPair;

  Request(struct evhttp_request *);
  Request(const Request &);
  Request &operator=(const Request &);
  ~Request();

  void setHeader(const std::string &name, const std::string &value);
  void writeBody(const std::string &content);
  void end();
  void end(const int code, const std::string &status);

  std::string &getHeader(const std::string &name);
  std::string getUri();
  std::string getMethod();
  struct evhttp_request *getRawRequest();

 private:
  void parse();

  HeaderMap headers_;
  std::string method_;
  std::string uri_;
  struct evkeyvalq *outputBufferHeaders_;
  struct evhttp_request *evhhtpRequestBase_;
  struct evbuffer *buffer_;
};

#endif //WEBSOCKET_REQUEST_HXX
