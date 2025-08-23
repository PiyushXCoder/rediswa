#pragma once
#include "connection.h"
#include <string>
#include <sys/types.h>

class Request {
public:
  Request(Connection &conn);
  void read();
  Connection &get_connection() const;
  Request &operator=(const Request &other);

  std::string body;
  ssize_t content_length;
  ssize_t read_length;
  bool is_read = false;

private:
  Connection &m_connection;
};
