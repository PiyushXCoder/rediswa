#pragma once
#include <sys/types.h>
#include <vector>

class Connection {
public:
  Connection(int fd);
  ~Connection();

  std::vector<std::byte> read_non_blocking();
  int write_non_blocking(const std::vector<std::byte> &data);

private:
  int m_fd;
};
