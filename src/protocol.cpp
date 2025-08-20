#include "protocol.h"
#include <arpa/inet.h>
#include <cstring>

SimpleProtocol::SimpleProtocol(Connection &connection)
    : m_connection(connection) {}

SimpleProtocol::~SimpleProtocol() {}

std::string SimpleProtocol::get_version() { return m_version; }

std::string SimpleProtocol::get_name() { return m_name; }

void SimpleProtocol::send_message(std::string message) {
  size_t buffer_length = message.size() + 4;
  char buffer[buffer_length];
  u_int32_t length = htonl(message.size());
  memcpy(buffer, &length, sizeof(length));
  memcpy(buffer + 4, message.c_str(), message.size());
  m_connection.write_full(buffer, buffer_length);
}

std::string SimpleProtocol::receive_message() {
  u_int32_t length = 0;
  m_connection.read_full((char *)&length, sizeof(length));
  length = ntohl(length);
  size_t buffer_length = length + 1;
  char buffer[buffer_length];
  m_connection.read_full(buffer, length);
  buffer[length] = '\0';
  return std::string(buffer);
}
