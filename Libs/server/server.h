#pragma once
#include "request.h"
#include "response.h"
#include "tcp_socket.h"
#include <functional>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

class MyProtocolServer {
public:
  MyProtocolServer(const char *ip, int port);
  ~MyProtocolServer();
  void add_route(Method method, Route route,
                 std::function<Response(Request)> handler);

  void start();

private:
  void event_loop();
  Response handle_request(const Request &request);

  TCPSocket m_socket;
  std::vector<Request> m_requests;
  std::vector<Response> m_responses;
  std::map<std::pair<Method, Route>, std::function<Response(Request)>>
      m_routes{};
};
