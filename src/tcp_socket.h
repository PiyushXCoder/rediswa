#pragma once

#include "connection.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <optional>
#include <sys/socket.h>
#include <sys/types.h>

class TCPSocket {
public:
  TCPSocket(const char *ip, int port);
  ~TCPSocket();

  std::optional<Connection> accept_connection();
  // void connect();

private:
  int m_fd;
  struct sockaddr_in m_address;
};
