#include "fd_utils.h"
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <unistd.h>

void fd_set_non_blocking(int fd) {
  errno = 0;
  int flags = fcntl(fd, F_GETFL, 0);
  if (errno) {
    throw std::runtime_error(std::string("fcntl error: ") + strerror(errno));
    return;
  }
  flags |= O_NONBLOCK;
  errno = 0;
  (void)fcntl(fd, F_SETFL, flags);
  if (errno) {
    throw std::runtime_error(std::string("fcntl error: ") + strerror(errno));
  }
}
