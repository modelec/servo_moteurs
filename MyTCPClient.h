#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

// #include "servo_motor.h"
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

#define SERVO_MIN    82 // 0.02*4096
#define SERVO_MAX    492 // 0.12*4096

enum BrasState {
    BRAS_BAS,
    BRAS_HAUT,
    BRAS_TRANSPORT
};

typedef struct {
    int servo4;
    int servo5;
}angleBras;

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
    BrasState positionBras = BRAS_BAS;
    bool pinceOuverte[3] = {true, true, true};
    angleBras angleBrasBas = {180, 0};
    angleBras angleBrasTransport = {168, 12};
    angleBras angleBrasHaut = {58, 122};//78, 102
};
