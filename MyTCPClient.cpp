#include "MyTCPClient.h"

MyTCPClient::MyTCPClient(const char *serverIP, int port) : TCPClient(serverIP, port), pca() {
    this->pwm_init();
}

void MyTCPClient::handleMessage(const std::string &message) {
    std::vector<std::string> token = TCPSocket::split(message, ";");

    if (token.size() != 4) {
        std::cerr << "Message invalide: " << message << std::endl;
        return;
    }

    if (token[1] == "servo_moteur" || token[1] == "all") {
        std::cout << message << std::endl;
        if (token[2] == "ping") {
            this->sendMessage("servo_moteur;ihm;pong;1");
        }
        else if (token[2] == "ouvrir pince") {
            int pince = std::stoi(token[3]);
            std::cout << "ouvrir pince : " << pince << std::endl;
            this->ouvrir_pince(pince);
        }
        else if (token[2] == "fermer pince") {
            int pince = std::stoi(token[3]);

            this->fermer_pince(pince);
        }
        else if (token[2] == "baisser bras") {
            this->baisser_bras();
        }
        else if (token[2] == "lever bras") {
            this->lever_bras();
        }
        else if (token[2] == "check panneau") {
            int bras = std::stoi(token[3]);
            this->check_panneau(bras);
        }
        else if (token[2] == "uncheck panneau") {
            int bras = std::stoi(token[3]);
            this->uncheck_panneau(bras);
        }
    }
}

void MyTCPClient::pwm_setFrequency(float freq) {
    pca.set_pwm_freq(freq);
}

void MyTCPClient::pwm_init() {
    pwm_setFrequency(50.0);
}


void MyTCPClient::pwm_setServoPosition(int servo, int position) {
    int on_time = SERVO_MIN + (SERVO_MAX - SERVO_MIN) * position / 180 - 1;//temps ou le servo est allumé par rapport à 4096
    pca.set_pwm(servo, 0, on_time);
}

void MyTCPClient::baisser_bras() {
    int angle = 100;
    for (int i = 1; i <= angle;i++){
        usleep(10'000);
        this->pwm_setServoPosition(4, i);
        this->pwm_setServoPosition(5, angle-i);
    }
}

void MyTCPClient::lever_bras() {
    int angle = 107;
    for (int i = 1; i <= angle;i++){
        usleep(10'000);
        this->pwm_setServoPosition(4, angle-i);
        this->pwm_setServoPosition(5, i);
    }
}

void MyTCPClient::fermer_pince(int pince) {
    int angle;
    if (pince < 0 || pince > 2){
        return;
    }
    switch(pince){
        case 0:
            angle = 137;
        break;
        case 1:
            angle = 42;
        break;
        case 2:
            angle = 152;
        break;
    }
    std::cout << "Fermer pince : " << pince << std::endl;
    this->pwm_setServoPosition(pince, angle);
}

void MyTCPClient::ouvrir_pince(int pince) {
    int angle;
    if (pince < 0 || pince > 2){
        return;
    }
    switch(pince){
        case 0:
            angle = 110;
        break;
        case 1:
            angle = 22;
        break;
        case 2:
            angle = 125;
        break;
    }
    std::cout << "Ouvrir pince : " << pince << std::endl;
    this->pwm_setServoPosition(pince, angle);
}


void MyTCPClient::check_panneau(int quelBras) {
    this->pwm_setServoPosition(quelBras, 30);
}

void MyTCPClient::uncheck_panneau(int quelBras) {
    this->pwm_setServoPosition(quelBras, 0);
}

void MyTCPClient::pwm_clear() {
    pca.set_all_pwm(0,0);
}
