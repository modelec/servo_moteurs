cmake_minimum_required(VERSION 3.25)
project(servo_motor)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TCPSocket REQUIRED TCPSocket)

add_executable(servo_motor servo_motor.cpp
        MyTCPClient.cpp
        MyTCPClient.h
        main.cpp)

target_link_libraries(servo_motor TCPSocket)