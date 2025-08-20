#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket: " << strerror(sockfd) << std::endl;
    return 1;
  }

  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
  server_addr.sin_port = ntohs(8000);

  if (connect(sockfd, (const sockaddr *)&server_addr, sizeof(server_addr))) {
    std::cerr << "Error connecting to server: " << strerror(errno) << std::endl;
    return 1;
  }

  const char *request = "big babool";
  u_int32_t req_len = strlen(request);
  char rbuf[4 + req_len];
  uint32_t req_len_net = htonl(req_len);
  memcpy(rbuf, &req_len_net, 4);
  memcpy(&rbuf[4], request, req_len);
  std::cout << "Sending request: " << request << std::endl;

  write(sockfd, rbuf, sizeof(rbuf));

  u_int32_t res_len = 0;
  ssize_t err = read(sockfd, &res_len, 4);
  res_len = ntohl(res_len);
  if (err < 0) {
    std::cerr << "Error reading response length: " << strerror(errno)
              << std::endl;
    close(sockfd);
    return 1;
  }
  char buf[res_len + 1];
  read(sockfd, &buf, sizeof(buf) - 1);
  buf[res_len] = '\0';
  std::cout << "Received response: " << std::string(buf) << std::endl;

  close(sockfd);
  return 0;
}
