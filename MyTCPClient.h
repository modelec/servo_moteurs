#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

// #include "servo_motor.h"
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

#define SERVO_MIN    82 // 0.02*4096
#define SERVO_MAX    492 // 0.12*4096
#define BRAS_BAS        0
#define BRAS_HAUT       1
#define BRAS_TRANSPORT  2
class MyTCPClient : public TCPClient {
public:
    explicit MyTCPClient(const char* serverIP = "127.0.0.1", int port = 8080);

    void handleMessage(const std::string &message) override;

    void pwm_setFrequency(float freq);

    void pwm_init();

    void pwm_setServoPosition(int servo, int position);

 //   void baisser_bras(bool force = false);
    
    void baisser_bras();

    void transport_bras();

//    void lever_bras(bool force = false);
    
    void lever_bras();

    void fermer_pince(int pince, bool force = false);

    void ouvrir_pince(int pince, bool force = false);

    void check_panneau(int quelBras);

    void uncheck_panneau(int quelBras);

    void pwm_clear();

private:
    PiPCA9685::PCA9685 pca;
    int positionBras;
    bool pinceOuverte[3] = {true, true, true};
    int brasBaisse = 0;
};
