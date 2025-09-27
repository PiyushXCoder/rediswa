#pragma once
#include "connection.h"
#include <array>
#include <optional>
#include <string>
#include <sys/types.h>

typedef struct {
  std::string method;
  std::string path;
  std::string version;
  std::string host;
  int content_length;
} RequestHeader;

class Request {
public:
  Request(Connection &conn);
  Connection &get_connection() const;
  Request &operator=(const Request &other);

  bool is_header_read() const;
  bool is_body_read() const;
  void read();
  const RequestHeader &get_header() const;
  const std::string &get_body() const;

private:
  Connection &m_connection;
  std::array<std::byte, sizeof(RequestHeader)> m_header_raw = {};
  std::optional<RequestHeader> m_header = std::nullopt;
  std::optional<std::string> m_body = std::nullopt;
  int m_read_length = 0;
};
