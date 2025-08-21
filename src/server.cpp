#include "server.h"
#include "protocol.h"
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

HttpServer::HttpServer(const char *ip, int port) : m_socket(ip, port) {}

HttpServer::~HttpServer() {}

void HttpServer::start() { event_loop(); }

void HttpServer::event_loop() {
  std::optional<Connection> connection = std::nullopt;
  while (connection == std::nullopt) {
    connection = m_socket.accept_connection();
  };
  std::cout << "Got Connection" << std::endl;

  SimpleProtocol protocol(*connection);
  std::cout << protocol.receive_message() << std::endl;
  protocol.send_message("Hello, World!");
  std::cout << "Sent response" << std::endl;
}
