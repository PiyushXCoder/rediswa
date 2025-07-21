#include "server.h"
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

HttpServer::HttpServer(const char *ip, int port) : m_socket(ip, port) {}

HttpServer::~HttpServer() {}

void HttpServer::start() { start_event_loop(); }

void HttpServer::start_event_loop() {
  std::optional<Connection> connection = std::nullopt;
  while (connection == std::nullopt) {
    connection = m_socket.accept_connection();
  };
  std::cout << "Got Connection" << std::endl;
  char buf[5];
  connection.value().read_full(buf, sizeof(buf));
  std::cout << std::string(buf) << std::endl;
}
