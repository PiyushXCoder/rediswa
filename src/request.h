#pragma once
#include "connection.h"
#include <string>
#include <sys/types.h>

enum Method : u_int8_t { NONE = 0, GET = 1, POST = 2, PUT = 3, DELETE = 4 };
typedef std::string Route;

class Request {
public:
  Request(Connection &conn);
  Connection &get_connection() const;
  Request &operator=(const Request &other);

  bool is_ready() const;
  void read();
  Method get_method() const;
  std::string get_route() const;
  std::string get_body() const;

private:
  Connection &m_connection;
  Method m_method = NONE;
  u_int32_t m_route_length = 0;
  Route m_route = "";
  u_int32_t m_body_length = 0;
  std::string m_body = "";
  int m_read_length = 0;

  u_int32_t m_get_total_length() const;
};
