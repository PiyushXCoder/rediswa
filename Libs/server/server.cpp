#include "server.h"
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <unistd.h>

MyProtocolServer::MyProtocolServer(const char *ip, int port)
    : m_socket(ip, port) {}

MyProtocolServer::~MyProtocolServer() {}

void MyProtocolServer::start() { event_loop(); }

void MyProtocolServer::event_loop() {
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

Response MyProtocolServer::handle_request(const Request &request) {
  auto route = m_routes.find({request.get_method(), request.get_route()});

  if (route != m_routes.end()) {
    return route->second(request);
  } else {
    return Response(request.get_connection(), 404, "Not Found");
  }
}

void MyProtocolServer::add_route(Method method, Route route,
                                 std::function<Response(Request)> handler) {
  m_routes[{method, route}] = handler;
}
