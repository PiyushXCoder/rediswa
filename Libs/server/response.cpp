#include "response.h"
#include <cstring>

Response::Response(Connection &conn, u_int16_t status, const std::string &body)
    : m_connection(conn), m_body(body), m_status(status),
      m_body_length(body.size()) {}
Connection &Response::get_connection() const { return m_connection; }

Response &Response::operator=(const Response &other) {
  if (this != &other) {
    this->m_status = other.m_status;
    this->m_body = other.m_body;
    this->m_written_length = other.m_written_length;
  }
  return *this;
}

bool Response::is_ready() const {
  return m_written_length >= m_get_total_length();
}

u_int32_t Response::m_get_total_length() const { return 2 + 4 + m_body_length; }

void Response::write() {
  if (m_written_length >= m_get_total_length())
    return;

  std::vector<std::byte> write(m_get_total_length());

  int field_start = 0;
  memcpy(write.data(), &m_status, sizeof(m_status));

  field_start += sizeof(m_status);
  memcpy(write.data() + field_start, &m_body_length, sizeof(m_body_length));

  field_start += sizeof(m_body_length);
  memcpy(write.data() + field_start, m_body.data(), m_body.size());

  int written = m_connection.write_non_blocking(write);
  m_written_length += written;
}
