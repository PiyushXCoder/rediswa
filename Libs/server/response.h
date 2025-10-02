#pragma once
#include "connection.h"
#include <string>
#include <sys/types.h>

class Response {
public:
  Response(Connection &conn, u_int16_t status, const std::string &body);
  Connection &get_connection() const;
  Response &operator=(const Response &other);

  bool is_ready() const;
  void write();

private:
  Connection &m_connection;
  u_int16_t m_status;
  u_int32_t m_body_length = 0;
  std::string m_body;
  int m_written_length = 0;

  u_int32_t m_get_total_length() const;
};
