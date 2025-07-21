#include "server.h"
#include <iostream>

int main(int argc, char *argv[]) {
  HttpServer http_server("0.0.0.0", 8000);

  try {
    http_server.start();
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
