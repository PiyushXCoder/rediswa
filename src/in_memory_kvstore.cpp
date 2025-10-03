#include "in_memory_kvstore.h"
#include <iostream>

InMemoryKVStore::InMemoryKVStore() {}

InMemoryKVStore::~InMemoryKVStore() {}

void InMemoryKVStore::put(const std::string &key, const std::string &value) {
  store[key] = value;
}

std::optional<std::string> InMemoryKVStore::get(const std::string &key) {
  auto it = store.find(key);
  if (it != store.end()) {
    return it->second;
  }
  return std::nullopt;
}

void InMemoryKVStore::remove(const std::string &key) { store.erase(key); }

bool InMemoryKVStore::exists(const std::string &key) {
  return store.find(key) != store.end();
}

size_t InMemoryKVStore::size() { return store.size(); }

void InMemoryKVStore::clear() { store.clear(); }

void InMemoryKVStore::print() {
  for (const auto &pair : store) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }
}
