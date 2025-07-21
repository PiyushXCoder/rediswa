#pragma once
#include <sys/types.h>

class Connection {
public:
  Connection(int fd);
  ~Connection();

  void read_full(char *buf, size_t n);
  void write_full(char *buf, size_t n);

private:
  int m_fd;
};
