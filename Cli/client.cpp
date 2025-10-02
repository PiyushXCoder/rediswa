#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0]
              << " <server_ip> <port> <method> <route> <body>" << std::endl;
    return 1;
  };

  const char *server_ip = argv[1];
  int port = atoi(argv[2]);
  const char *route = argv[4];
  const char *body = argv[5];

  u_int8_t method;
  if (strcmp(argv[3], "GET") == 0) {
    method = 1;
  } else if (strcmp(argv[3], "POST") == 0) {
    method = 2;
  } else if (strcmp(argv[3], "PUT") == 0) {
    method = 3;
  } else if (strcmp(argv[3], "DELETE") == 0) {
    method = 4;
  } else {
    std::cerr << "Invalid method: " << argv[3] << std::endl;
    return 1;
  }

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket: " << strerror(sockfd) << std::endl;
    return 1;
  }

  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = ntohs(port);

  if (connect(sockfd, (const sockaddr *)&server_addr, sizeof(server_addr))) {
    std::cerr << "Error connecting to server: " << strerror(errno) << std::endl;
    return 1;
  }

  //
  // Send request
  //
  u_int32_t route_length = strlen(route);
  u_int32_t body_length = strlen(body);
  char rbuf[1 + 4 + route_length + 4 + body_length];

  int field_start = 0;
  rbuf[field_start] = 1; // Method: GET

  field_start += 1;
  u_int32_t len = htonl(route_length);
  memcpy(&rbuf[field_start], &len, 4);

  field_start += 4;
  memcpy(&rbuf[field_start], route, route_length);

  field_start += route_length;
  len = htonl(body_length);
  memcpy(&rbuf[field_start], &len, 4);

  field_start += 4;
  memcpy(&rbuf[field_start], body, body_length);

  write(sockfd, rbuf, sizeof(rbuf));

  //
  // Read response
  //
  u_int16_t status = 0;
  ssize_t err = read(sockfd, &status, 2);
  if (err < 0) {
    std::cerr << "Error reading response length: " << strerror(errno)
              << std::endl;
    close(sockfd);
    return 1;
  }
  status = status;
  std::cout << "Received status: " << status << std::endl;

  u_int32_t res_len = 0;
  err = read(sockfd, &res_len, 4);
  if (err < 0) {
    std::cerr << "Error reading response length: " << strerror(errno)
              << std::endl;
    close(sockfd);
    return 1;
  }
  res_len = res_len;
  std::cout << "Response length: " << res_len << std::endl;

  char buf[res_len + 1];
  read(sockfd, &buf, sizeof(buf) - 1);
  buf[res_len] = '\0';
  std::cout << "Received response: " << std::string(buf) << std::endl;

  close(sockfd);
  return 0;
}
