#pragma once
#include "connection.h"
#include <string>

template <typename T> class Protocol {
public:
  virtual ~Protocol() {};
  virtual std::string get_version() = 0;
  virtual std::string get_name() = 0;
  virtual void send_message(T) = 0;
  virtual T receive_message() = 0;
};

class SimpleProtocol : public Protocol<std::string> {
public:
  SimpleProtocol(Connection &connection);
  ~SimpleProtocol() override;
  std::string get_version() override;
  std::string get_name() override;
  void send_message(std::string message) override;
  std::string receive_message() override;

private:
  Connection &m_connection;
  std::string m_version = "1.0";
  std::string m_name = "SimpleProtocol";
};
