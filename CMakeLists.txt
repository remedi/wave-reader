cmake_minimum_required(VERSION 3.15)
project(wave_reader CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(client1 client1.cc Socket.cc TcpReader.cc)
target_include_directories(client1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
