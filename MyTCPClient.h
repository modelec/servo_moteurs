#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

// #include "servo_motor.h"
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

#define SERVO_MIN    82 // 0.02*4096
#define SERVO_MAX    492 // 0.12*4096

class MyTCPClient : public TCPClient {
public:
    explicit MyTCPClient(const char* serverIP = "127.0.0.1", int port = 8080);

    void handleMessage(const std::string &message) override;

    void pwm_setFrequency(float freq);

    void pwm_init();

    void pwm_setServoPosition(int servo, int position);

    void baisser_bras();

    void lever_bras();

    void fermer_pince(int pince);

    void ouvrir_pince(int pince);

    void check_panneau(int quelBras);

    void uncheck_panneau(int quelBras);

private:
    PiPCA9685::PCA9685 pca;
};
