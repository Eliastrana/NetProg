cmake_minimum_required(VERSION 3.27)
project(oving3)

set(CMAKE_CXX_STANDARD 17)

add_executable(oving3 main.cpp
        Server.cpp
        Client.cpp, WebServer.cpp)
