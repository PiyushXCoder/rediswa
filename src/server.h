#pragma once
#include "tcp_socket.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class HttpServer {
public:
  HttpServer(const char *ip, int port);
  ~HttpServer();

  void start();

private:
  void start_event_loop();

  TCPSocket m_socket;
};
