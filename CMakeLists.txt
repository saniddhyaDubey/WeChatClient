cmake_minimum_required(VERSION 3.10)
project(WeChatClient LANGUAGES CXX)

set(SOURCES
    ./src/main.cpp
    ./src/unix_client.cpp
)

add_executable(WeChatClient ${SOURCES})

target_include_directories(WeChatClient PRIVATE ${CMAKE_SOURCE_DIR}/include)
