cmake_minimum_required(VERSION 3.20)

project(rediswa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rediswa
    src/main.cpp
    src/server.cpp
    src/server.h
    src/tcp_socket.cpp
    src/tcp_socket.h
    src/connection.cpp
    src/connection.h
    src/fd_utils.cpp
    src/fd_utils.h
    src/protocol.cpp
    src/protocol.h
)

add_executable(rediswa-client
    src/client.cpp
)

