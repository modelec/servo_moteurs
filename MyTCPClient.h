#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

#include "servo_motor.h"

class MyTCPClient : public TCPClient {
public:
    explicit MyTCPClient(const char* serverIP = "127.0.0.1", int port = 8080);

    ~MyTCPClient() override;

    int init();

    void handleMessage(const std::string &message) override;

private:
    int handle;
};
