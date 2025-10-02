target_sources(ServerLib PRIVATE 
    server.cpp
    server.h
    tcp_socket.cpp
    tcp_socket.h
    connection.cpp
    connection.h
    fd_utils.cpp
    fd_utils.h
    request.h
    request.cpp
    response.h
    response.cpp
)

target_include_directories(ServerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
