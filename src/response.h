#pragma once
#include "connection.h"
#include <string>
#include <sys/types.h>

class Response {
public:
  Response(Connection &conn);
  void set_content_length(ssize_t length);
  void set_body(const std::string &body);
  void write();
  Connection &get_connection() const;
  Response &operator=(const Response &other);

  std::string body;
  ssize_t content_length;
  ssize_t write_length;
  bool is_written = false;

private:
  Connection &m_connection;
};
