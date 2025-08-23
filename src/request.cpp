#include "request.h"
#include "protocol.h"

Request::Request(Connection &conn)
    : m_connection(conn), content_length(-1), read_length(0) {}

void Request::read() {
  if (is_read)
    return;

  SimpleProtocol protocol(this->m_connection);
  protocol.receive_message();
}

Request &Request::operator=(const Request &other) {
  if (this != &other) {
    this->m_connection = other.m_connection;
    this->body = other.body;
    this->content_length = other.content_length;
    this->read_length = other.read_length;
    this->is_read = other.is_read;
  }
  return *this;
}
