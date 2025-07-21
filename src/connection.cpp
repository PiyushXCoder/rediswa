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

void Connection::read_full(char *buf, size_t n) {
  while (n > 0) {
    ssize_t read_count = read(m_fd, buf, n);
    if (read_count <= 0)
      throw std::runtime_error("Reading failed");
    if (read_count > (ssize_t)n)
      throw std::runtime_error("Buffer over flow while read");

    n -= (size_t)read_count;
    buf += read_count;
  }
}

void Connection::write_full(char *buf, size_t n) {
  while (n > 0) {
    ssize_t write_count = write(m_fd, buf, n);
    if (write_count <= 0)
      throw std::runtime_error("Reading failed");
    if (write_count > (ssize_t)n)
      throw std::runtime_error("Buffer over flow while write");
    n -= (size_t)write_count;
    buf += write_count;
  }
}
