#pragma once
#include <optional>
#include <string>

class KVStore {
public:
  virtual void put(const std::string &key, const std::string &value) = 0;
  virtual std::optional<std::string> get(const std::string &key) = 0;
  virtual void remove(const std::string &key) = 0;
  virtual bool exists(const std::string &key) = 0;
  virtual size_t size() = 0;
  virtual void clear() = 0;
  virtual void print() = 0;

  virtual ~KVStore() {};
};
