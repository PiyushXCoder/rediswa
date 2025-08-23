#pragma once
#include <optional>
#include <string>
#include <sys/types.h>

class Connection {
public:
  Connection(int fd);
  ~Connection();

  void read_full(char *buf, size_t n);
  void write_full(char *buf, size_t n);

  std::optional<std::string> read_non_blocking();
  int write_non_blocking(const std::string &data);

private:
  int m_fd;
};
