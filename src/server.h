#pragma once
#include "request.h"
#include "response.h"
#include "tcp_socket.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

class HttpServer {
public:
  HttpServer(const char *ip, int port);
  ~HttpServer();

  void start();

private:
  void event_loop();
  Response handle_request(const Request &request);

  TCPSocket m_socket;
  std::vector<Request> m_requests;
  std::vector<Response> m_responses;
};
