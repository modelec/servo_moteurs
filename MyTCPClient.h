#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

// #include "servo_motor.h"
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

#define SERVO_MIN    82 // 0.02*4096
#define SERVO_MAX    492 // 0.12*4096
#define PANO_HAUT    25
#define PANO_BAS     5


enum BrasState {
    BRAS_BAS,
    BRAS_HAUT,
    BRAS_TRANSPORT
};

enum PinceState {
    PINCE_OUVERTE,
    PINCE_MIDDLE,
    PINCE_FERMER,
};

typedef struct {
    int servo4;
    int servo5;
}angleBras;

typedef struct {
    int fermer;
    int middle;
    int ouverte;
    int fullyOuverte;
}anglePince;

typedef struct {
    int check;
    int uncheck;
}anglePanneau;

class MyTCPClient : public TCPClient {
public:
    explicit MyTCPClient(const char* serverIP = "127.0.0.1", int port = 8080);

    void handleMessage(const std::string &message) override;

    void pwm_setFrequency(float freq);

    void pwm_init();

    void pwm_setServoPosition(int servo, int position);

    void baisser_bras();

    void transport_bras();

    void lever_bras();

    void fermer_pince(int pince, bool force = false);

    void ouvrir_pince(int pince, bool force = false);

    void middle_pince(int pince, bool force = false);

    void ouvrir_total_pince(int pince, bool force = false);

    void check_panneau(int quelBras);

    void uncheck_panneau(int quelBras);

    void percentage_panneau(int quelBras, int pourcentage);

    void pwm_clear();

private:
    PiPCA9685::PCA9685 pca;
    BrasState positionBras = BRAS_BAS;
    int etatPince[3] = {PINCE_OUVERTE, PINCE_OUVERTE, PINCE_OUVERTE};

    const angleBras angleBrasBas = {180, 0};
    const angleBras angleBrasTransport = {168, 12};
    const angleBras angleBrasHaut = {53, 127};//78, 102

    // const anglePince anglePince0 = {142, 129, 108};//Ecart -27
    // const anglePince anglePince1 = {42, 32, 15};//Ecart -20
    // const anglePince anglePince2 = {152, 141, 122};//Ecart -22

    const anglePince anglePince0 = {142, 129, 118, 108};//Ecart -27
    const anglePince anglePince1 = {42, 32, 19, 15};//Ecart -20
    const anglePince anglePince2 = {152, 141, 125, 122};//Ecart -22

    const anglePanneau anglePanneauGauche = {16, 40};
    const anglePanneau anglePanneauDroit = {25, 3};
};
