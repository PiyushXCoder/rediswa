cmake_minimum_required(VERSION 3.20)

project(rediswa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ServerLib)

add_subdirectory(Libs/server)

add_executable(rediswa
    src/main.cpp
    src/in_memory_kvstore.cpp
    src/routes.cpp
)

target_link_libraries(rediswa PRIVATE ServerLib)

add_executable(hello
    Tests/hello.cpp
)

add_executable(client
    Cli/client.cpp
)

