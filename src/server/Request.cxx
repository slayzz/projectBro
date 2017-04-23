
#include <iostream>
#include "Request.hxx"
Request::Request(struct evhttp_request *req) :
    evhhtpRequestBase_(req), uri_(evhttp_request_get_uri(req)),
    outputBufferHeaders_(evhttp_request_get_output_headers(req)),
    buffer_(evbuffer_new()) {
  defaults();
  parse();
}

Request::~Request() {
  evbuffer_free(buffer_);
}

Request::Request(Request &&that) {
  headers_ = std::move(that.headers_);
  method_ = std::move(that.method_);
  uri_ = std::move(that.uri_);
  outputBufferHeaders_ = that.outputBufferHeaders_;
  evhhtpRequestBase_ = that.evhhtpRequestBase_;
  buffer_ = that.buffer_;

  that.outputBufferHeaders_ = nullptr;
  that.evhhtpRequestBase_ = nullptr;
  that.buffer_ = nullptr;
}

Request &Request::operator=(Request &&that) {
  if (this != &that) {
    headers_ = std::move(that.headers_);
    method_ = std::move(that.method_);
    uri_ = std::move(that.uri_);
    outputBufferHeaders_ = that.outputBufferHeaders_;
    evhhtpRequestBase_ = that.evhhtpRequestBase_;
    buffer_ = that.buffer_;

    that.outputBufferHeaders_ = nullptr;
    that.evhhtpRequestBase_ = nullptr;
    that.buffer_ = nullptr;

  }
  return *this;
}

void Request::parse() {
  const auto headers = evhttp_request_get_input_headers(evhhtpRequestBase_);
  switch (evhttp_request_get_command(evhhtpRequestBase_)) {
    case EVHTTP_REQ_GET: method_ = "GET";
      break;
    case EVHTTP_REQ_POST: method_ = "POST";
      break;
    case EVHTTP_REQ_HEAD: method_ = "HEAD";
      break;
    case EVHTTP_REQ_PUT: method_ = "PUT";
      break;
    case EVHTTP_REQ_DELETE: method_ = "DELETE";
      break;
    case EVHTTP_REQ_OPTIONS: method_ = "OPTIONS";
      break;
    case EVHTTP_REQ_TRACE: method_ = "TRACE";
      break;
    case EVHTTP_REQ_CONNECT: method_ = "CONNECT";
      break;
    case EVHTTP_REQ_PATCH: method_ = "PATCH";
      break;
    default: method_ = "unknown";
      break;
  }

  for (auto header = headers->tqh_first; header;
       header = header->next.tqe_next) {
    headers_.insert(HeaderPair(header->key, header->value));
  }
}


const std::string Request::getHeader(const std::string &name) const {
  auto header = headers_.find(name);
  if(header == headers_.end()) {
    return std::string("");
  }
  return header->second;
}

std::string Request::getUri() {
  return uri_;
}

std::string Request::getMethod() {
  return method_;
}

void Request::end() {
  evhttp_send_reply(evhhtpRequestBase_, HTTP_OK, "OK", buffer_);
}

void Request::end(const int code, const std::string &status) {
  evhttp_send_reply(evhhtpRequestBase_, code, status.c_str(), buffer_);
}

void Request::setHeader(const std::string &name, const std::string &value) {
  evhttp_add_header(outputBufferHeaders_, name.c_str(), value.c_str());
}

struct evhttp_request *Request::getRawRequest() {
  return evhhtpRequestBase_;
}

void Request::writeBody(const std::string &content) {
  evbuffer_add(buffer_, content.c_str(), content.length());
}

void Request::defaults() {
  setHeader("Server", Globals::SERVER_NAME);
}

