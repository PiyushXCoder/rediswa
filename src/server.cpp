#include "server.h"
#include <arpa/inet.h>
#include <iostream>
#include <thread>
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
      if (!request->is_ready()) {
        try {
          request->read();
        } catch (const std::exception &e) {
          std::cerr << "Error reading request: " << e.what() << std::endl;
          continue;
        }
      } else {
        auto response = this->handle_request(*request);
        m_responses.push_back(response);
        request = m_requests.erase(request);
        continue;
      }
      ++request;
    }

    // Handle responses
    for (auto response = m_responses.begin(); response != m_responses.end();) {
      if (!response->is_ready()) {
        try {
          response->write();
        } catch (const std::exception &e) {
          std::cerr << "Error writing response: " << e.what() << std::endl;
          continue;
        }
      } else {
        response = m_responses.erase(response);
        continue;
      }
      ++response;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

Response HttpServer::handle_request(const Request &request) {
  std::cout << "Handling request: " << request.get_method() << " "
            << request.get_path() << std::endl;
  std::cout << "Body: " << std::endl << request.get_body() << std::endl;
  return Response(request.get_connection(), 200, "Hello World");
}
