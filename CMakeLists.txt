cmake_minimum_required(VERSION 3.25)
project(servo_motor)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
find_package(PiPCA9685 REQUIRED)
pkg_check_modules(TCPSocket REQUIRED TCPSocket)

add_executable(servo_motor
        MyTCPClient.cpp
        MyTCPClient.h
        main.cpp)

add_executable(test
    test.main.cpp
)

target_link_libraries(servo_motor TCPSocket)
target_link_libraries(servo_motor ${PiPCA9685_LIBRARY})
target_link_libraries(test ${PiPCA9685_LIBRARY})