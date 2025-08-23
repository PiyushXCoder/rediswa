#include "server.h"
#include "protocol.h"
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

HttpServer::HttpServer(const char *ip, int port) : m_socket(ip, port) {}

HttpServer::~HttpServer() {}

void HttpServer::start() { event_loop(); }

void HttpServer::event_loop() {
  while (true) {
    // Accept new connections
    std::optional<Connection> connection = m_socket.accept_connection();
    if (connection != std::nullopt) {
      Request request(*connection);
      m_requests.push_back(request);
    };

    // Handle  requests
    for (auto request = m_requests.begin(); request != m_requests.end();) {
      if (!request->is_read) {
        try {
          request->read();
        } catch (const std::exception &e) {
          std::cerr << "Error reading request: " << e.what() << std::endl;
          continue;
        }
      }
      if (request->is_read) {
        auto response = this->handle_request(*request);
        m_responses.push_back(response);
        request = m_requests.erase(request);
      } else {
        ++request;
      }
    }

    // Handle responses
    for (auto response = m_responses.begin(); response != m_responses.end();) {
      if (!response->is_written) {
        try {
          response->write();
        } catch (const std::exception &e) {
          std::cerr << "Error writing response: " << e.what() << std::endl;
          continue;
        }
      }
      if (response->is_written) {
        response = m_responses.erase(response);
      } else {
        ++response;
      }
    }
  }
}
