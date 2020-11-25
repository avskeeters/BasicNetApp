cmake_minimum_required(VERSION 3.0)

project(BasicNetAppTests VERSION 0.0 LANGUAGES CXX)

find_package(NetworkingLib REQUIRED)

add_executable(HelloWorldTests src/HelloWorld.cpp)

target_link_libraries(HelloWorldTests PRIVATE MyLibraries::NetworkingLib)

