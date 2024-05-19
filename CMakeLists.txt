cmake_minimum_required(VERSION 3.25)
project(servo_motor)

set(CMAKE_CXX_STANDARD 17)

find_package(PiPCA9685 REQUIRED)

find_package(Modelec COMPONENTS
        TCPClient
        REQUIRED
)

add_executable(servo_motor
        MyTCPClient.cpp
        MyTCPClient.h
        main.cpp)

target_link_libraries(servo_motor TCPClient)
target_link_libraries(servo_motor ${PiPCA9685_LIBRARY})
