#include <cassert>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

const size_t k_max_msg = 4098;

void dosomething(int sockfd);
int32_t read_full(int fd, char *buf, size_t n);
int32_t write_full(int fd, char *buf, size_t n);
int32_t one_request(int fd);

int main(int argc, char *argv[]) {
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket: " << strerror(sockfd) << std::endl;
    return 1;
  }
  int optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);

  if (bind(sockfd, (const sockaddr *)&server_addr, sizeof(server_addr))) {
    std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
    return 1;
  }

  if (listen(sockfd, SOMAXCONN)) {
    std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
    return 1;
  }

  while (true) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_sockfd =
        accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sockfd < 0) {
      std::cerr << "Error accepting connection: " << strerror(errno)
                << std::endl;
      return 1;
    }

    dosomething(client_sockfd);
    close(client_sockfd);
  }

  return 0;
}

void dosomething(int sockfd) { one_request(sockfd); }

int32_t read_full(int fd, char *buf, size_t n) {
  while (n > 0) {
    ssize_t read_count = read(fd, buf, n);
    if (read_count <= 0)
      return -1;
    assert(read_count <= (ssize_t)n);
    n -= (size_t)read_count;
    buf += read_count;
  }
  return 0;
}

int32_t write_full(int fd, char *buf, size_t n) {
  while (n > 0) {
    ssize_t write_count = write(fd, buf, n);
    if (write_count <= 0)
      return -1;
    assert(write_count <= (ssize_t)n);
    n -= (size_t)write_count;
    buf += write_count;
  }
  return 0;
}

int32_t one_request(int fd) {
  char read_buf[4 + k_max_msg + 1] = {};
  errno = 0;
  int32_t err = read_full(fd, read_buf, 4);
  if (err) {
    if (errno == 0)
      std::cout << "EOF" << std::endl;
    else
      std::cout << "read failed" << std::endl;
    return err;
  }

  u_int32_t req_len = 0;
  memcpy(&req_len, read_buf, 4);
  std::cout << "Message length: " << req_len << std::endl;
  if (req_len > k_max_msg) {
    std::cout << "Too long message" << std::endl;
    return -1;
  }

  // read body
  err = read_full(fd, &read_buf[4], req_len);
  if (err) {
    std::cout << "read failed" << std::endl;
    return err;
  }

  read_buf[4 + req_len] = '\0';
  std::cout << "Received message: " << &read_buf[4] << std::endl;

  const char *reply = "World";
  u_int32_t res_len = strlen(reply);
  char wbuf[4 + res_len];
  memcpy(wbuf, &res_len, 4);
  memcpy(&wbuf[4], reply, res_len);
  err = write_full(fd, wbuf, (size_t)sizeof(wbuf));
  std::cout << "Reply sent" << std::endl;
  if (err) {
    std::cout << "write failed" << std::endl;
    return err;
  }

  return 0;
}
