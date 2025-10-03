#include "routes.h"
#include "server.h"
#include <iostream>

int main(int argc, char *argv[]) {
  MyProtocolServer my_protocol_server("0.0.0.0", 8000);
  configure_routes(my_protocol_server);

  try {
    my_protocol_server.start();
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
