#include "routes.h"
#include "in_memory_kvstore.h"
#include "kvstore.h"
#include <cstdio>
#include <iostream>
#include <mutex>

static std::mutex kv_mutex;
static KVStore *global_kvstore = (KVStore *)new InMemoryKVStore();

void configure_routes(MyProtocolServer &server) {
  server.add_route(Method::GET, "/hello", hello);

  // Configure the /kvstore route
  server.add_route(Method::GET, "/kvstore", get_value);
  server.add_route(Method::POST, "/kvstore", add_value);
  server.add_route(Method::PUT, "/kvstore", update_value);
  server.add_route(Method::DELETE, "/kvstore", delete_value);
}

Response hello(Request req) {
  return Response(req, 200, "What kind of hello do you want?");
}

Response get_value(Request req) {
  char key[100];
  std::sscanf(req.get_body().c_str(), "Key:%s", key);

  std::lock_guard<std::mutex> lock(kv_mutex);
  auto value = global_kvstore->get(key);

  if (value) {
    return Response(req, 200, *value);
  } else {
    return Response(req, 404, "Key not found");
  }
}

Response add_value(Request req) {
  std::cout << "Request Body: " << req.get_body() << std::endl;
  char key[1024], value[1024];
  sscanf(req.get_body().c_str(), "Key:%s Value:%s", key, value);

  std::lock_guard<std::mutex> lock(kv_mutex);
  if (global_kvstore->exists(key)) {
    return Response(req, 409, "Key already exists");
  } else {
    global_kvstore->put(key, value);
    return Response(req, 201, "Key-Value pair added");
  }
}

Response update_value(Request req) {
  char key[1024], value[1024];
  sscanf(req.get_body().c_str(), "Key:%s Value:%s", key, value);

  std::lock_guard<std::mutex> lock(kv_mutex);
  if (global_kvstore->exists(key)) {
    global_kvstore->put(key, value);
    return Response(req, 200, "Key-Value pair updated");
  } else {
    return Response(req, 404, "Key not found");
  }
}

Response delete_value(Request req) {
  char key[1024];
  sscanf(req.get_body().c_str(), "Key:%s", key);

  std::lock_guard<std::mutex> lock(kv_mutex);
  if (global_kvstore->exists(key)) {
    global_kvstore->remove(key);
    return Response(req, 200, "Key-Value pair deleted");
  } else {
    return Response(req, 404, "Key not found");
  }
}
