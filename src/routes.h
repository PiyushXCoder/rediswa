#pragma once
#include "server.h"

void configure_routes(MyProtocolServer &server);

Response hello(Request req);

Response get_value(Request req);
Response add_value(Request req);
Response update_value(Request req);
Response delete_value(Request req);
