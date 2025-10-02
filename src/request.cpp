#include "request.h"

Request::Request(Connection &conn) : m_connection(conn) {}

Connection &Request::get_connection() const { return m_connection; }

Request &Request::operator=(const Request &other) {
  if (this != &other) {
    this->m_method = other.m_method;
    this->m_path_length = other.m_path_length;
    this->m_path = other.m_path;
    this->m_body_length = other.m_body_length;
    this->m_body = other.m_body;
    this->m_read_length = other.m_read_length;
  }
  return *this;
}

Method Request::get_method() const { return (Method)m_method; }

std::string Request::get_path() const { return m_path; }

std::string Request::get_body() const { return m_body; }

u_int32_t Request::m_get_total_length() const {
  return 1 + 4 + m_path_length + 4 + m_body_length;
}

bool Request::is_ready() const { return m_read_length >= m_get_total_length(); }

void Request::read() {
  if (m_read_length >= m_get_total_length())
    return;

  auto read = m_connection.read_non_blocking();

  if (read.empty())
    return;

  int field_start = 0;
  int field_length = sizeof(m_method);
  if (m_read_length <= field_start) {
    m_method = static_cast<Method>(read[0]);
    m_read_length += 1;
    read = std::vector(read.begin() + field_length, read.end());
  }

  field_start += field_length;
  field_length = sizeof(m_path_length);
  if (m_read_length >= field_start &&
      m_read_length <= field_start + field_length) {
    int skip = m_read_length - field_start;
    for (int i = skip; i < field_length && i < read.size(); i++) {
      m_path_length = (m_path_length << 8) | (u_int32_t)read[i];
      m_read_length += 1;
    }
    read = std::vector(read.begin() + (field_length - skip), read.end());
  }

  field_start += field_length;
  field_length = m_path_length;
  if (m_read_length >= field_start &&
      m_read_length <= field_start + field_length) {
    int skip = m_read_length - field_start;
    for (int i = skip; i < field_length && i < read.size(); i++) {
      m_path += (char)read[i];
      m_read_length += 1;
    }
    read = std::vector(read.begin() + (field_length - skip), read.end());
  }

  field_start += field_length;
  field_length = sizeof(m_body_length);
  if (m_read_length >= field_start &&
      m_read_length <= field_start + field_length) {
    int skip = m_read_length - field_start;
    for (int i = skip; i < field_length && i < read.size(); i++) {
      m_body_length = (m_body_length << 8) | (u_int32_t)read[i];
      m_read_length += 1;
    }
    read = std::vector(read.begin() + (field_length - skip), read.end());
  }

  field_start += field_length;
  field_length = m_body_length;
  if (m_read_length >= field_start &&
      m_read_length <= field_start + field_length) {
    int skip = m_read_length - field_start;
    for (int i = skip; i < field_length && i < read.size(); i++) {
      m_body += (char)read[i];
      m_read_length += 1;
    }
    read = std::vector(read.begin() + (field_length - skip), read.end());
  }
}
