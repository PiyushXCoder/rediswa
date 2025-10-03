target_sources(ServerLib PRIVATE 
    server.cpp
    tcp_socket.cpp
    connection.cpp
    fd_utils.cpp
    request.cpp
    response.cpp
)

target_include_directories(ServerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
