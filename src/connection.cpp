#include "connection.h"
#include "fd_utils.h"
#include <iostream>
#include <string.h>
#include <unistd.h>

Connection::Connection(int fd) : m_fd(fd) { fd_set_non_blocking(m_fd); }

Connection::~Connection() {
  if (m_fd == -1) {
    if (close(m_fd) < 0) {
      std::cout << "Failed listening to socket: " << strerror(errno)
                << std::endl;
      std::exit(1);
    }
  }
}

std::vector<std::byte> Connection::read_non_blocking() {
  std::vector<std::byte> buffer(1024);
  ssize_t bytes_read = read(m_fd, buffer.data(), buffer.size());
  if (bytes_read < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // No data available right now
      return {};
    } else {
      throw std::runtime_error("Reading failed: " +
                               std::string(strerror(errno)));
    }
  } else if (bytes_read == 0) {
    // Connection closed
    return {};
  }
  buffer.resize(bytes_read);
  return buffer;
}

int Connection::write_non_blocking(const std::vector<std::byte> &data) {
  ssize_t bytes_written = write(m_fd, data.data(), data.size());
  if (bytes_written < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // Cannot write right now
      return 0;
    } else {
      throw std::runtime_error("Writing failed: " +
                               std::string(strerror(errno)));
    }
  }
  return bytes_written;
}
