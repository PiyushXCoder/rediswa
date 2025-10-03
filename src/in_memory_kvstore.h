#pragma once
#include "kvstore.h"
#include <map>

class InMemoryKVStore : public KVStore {
public:
  InMemoryKVStore();
  ~InMemoryKVStore();

  void put(const std::string &key, const std::string &value) override;
  std::optional<std::string> get(const std::string &key) override;
  void remove(const std::string &key) override;
  bool exists(const std::string &key) override;
  size_t size() override;
  void clear() override;
  void print() override;

private:
  std::map<std::string, std::string> store;
};
