#ifndef WEBSOCKET_REQUEST_HXX
#define WEBSOCKET_REQUEST_HXX

#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <string>
#include <map>
#include "../globals.hxx"

class Request final {
 public:
  typedef std::map<std::string, std::string> HeaderMap;
  typedef std::pair<std::string, std::string> HeaderPair;

  Request(struct evhttp_request *);
  ~Request();
  Request(Request &&that);
  Request &operator=(Request &&that);
  Request(const Request &) = delete;
  Request &operator=(const Request &) = delete;

  void setHeader(const std::string &name, const std::string &value);
  void writeBody(const std::string &content);
  void end();
  void end(const int code, const std::string &status);

  const std::string getHeader(const std::string &name) const;
  std::string getUri();
  std::string getMethod();
  struct evhttp_request *getRawRequest();

 private:
  HeaderMap headers_;
  std::string method_;
  std::string uri_;
  struct evkeyvalq *outputBufferHeaders_;
  struct evhttp_request *evhhtpRequestBase_;
  struct evbuffer *buffer_;

  void parse();
  void defaults();
};

#endif //WEBSOCKET_REQUEST_HXX
