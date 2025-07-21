#include "tcp_socket.h"
#include "fd_utils.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

TCPSocket::TCPSocket(const char *ip, int port) : m_fd(-1), m_address() {
  m_address.sin_port = htons(port);
  m_address.sin_family = AF_INET;
  if (inet_pton(AF_INET, ip, &m_address.sin_addr) <= 0) {
    throw std::runtime_error("Invalid address");
  }

  m_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (m_fd < 0) {
    throw std::runtime_error(std::string("Error creating socket: ") +
                             strerror(errno));
  }

  int optval = 1;
  setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (bind(m_fd, (const sockaddr *)&m_address, sizeof(m_address))) {
    throw std::runtime_error(std::string("Error binging socket: ") +
                             strerror(errno));
  }

  if (listen(m_fd, SOMAXCONN)) {
    throw std::runtime_error(std::string("Failed listening to socket: ") +
                             strerror(errno));
  }

  fd_set_non_blocking(m_fd);
}

TCPSocket::~TCPSocket() {
  if (m_fd == -1) {
    if (close(m_fd) < 0) {
      std::cout << "Failed listening to socket: " << strerror(errno)
                << std::endl;
      std::exit(1);
    }
  }
}

std::optional<Connection> TCPSocket::accept_connection() {
  socklen_t add_len = sizeof(m_address);
  int client_sockfd = accept(m_fd, (struct sockaddr *)&m_address, &add_len);
  if (client_sockfd < 0) {
    if (errno == EAGAIN) {
      return std::nullopt;
    } else if (errno < 0) {
      throw std::runtime_error(std::string("Failed accepting connection: ") +
                               strerror(errno));
    }
  }
  return Connection(client_sockfd);
}
