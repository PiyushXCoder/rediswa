#include "request.h"
#include "response.h"
#include "server.h"
#include <iostream>

Response hello_handler(Request req) {
  return Response(req, 200, "What kind of hello do you want?");
}

int main(int argc, char *argv[]) {
  MyProtocolServer my_protocol_server("0.0.0.0", 8000);
  my_protocol_server.add_route(Method::GET, "/hello", hello_handler);

  try {
    my_protocol_server.start();
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
