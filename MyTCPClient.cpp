#include "MyTCPClient.h"

MyTCPClient::MyTCPClient(const char *serverIP, int port) : TCPClient(serverIP, port) {
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
            this->sendMessage("servo_pot;ihm;pong;1");
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
    // pca.set_pwm(servo, 0, on_time);
    std::cout << (0 & 0xFF) << std::endl;
    std::cout << (0 >> 8) << std::endl;
    std::cout << (on_time & 0xFF) << std::endl;
    std::cout << (on_time >> 8) << std::endl;

    int on_time_lsb = on_time & ~(0b1111 << 8);// 8 premiers bits de poids faible du temps ou le servo est allumé
    int on_time_msb = on_time >> 8;// 4 premiers bits poids fort de on_time
    // i2cWriteWordData(handle, LED0_ON_L + 4 * channel, 0);
    std::cout << 0 << std::endl;
    // i2cWriteByteData(handle, LED0_ON_H + 4* channel, 0);
    std::cout << 0 << std::endl;
    std::cout << on_time_lsb << std::endl;
    // i2cWriteByteData(handle, LED0_OFF_L + 4 * channel, on_time_lsb);//0xCD = 0.05*2^12-1
    if (0 < (on_time >> 8) && (on_time >> 8) < 256){
        // i2cWriteByteData(handle, LED0_OFF_H + 4*channel, on_time_msb);
        std::cout << on_time_msb << std::endl;
    }
    else {
        // i2cWriteByteData(handle, LED0_OFF_H +4*channel, 0);
        std::cout << 0 << std::endl;
    }
}

void MyTCPClient::baisser_bras() {
    int angle = 100;
    for (int i = 1; i <= angle;i++){
        usleep(100'000);
        this->pwm_setServoPosition(4, i);
        this->pwm_setServoPosition(5, angle-i);
    }
}

void MyTCPClient::lever_bras() {
    int angle = 107;
    for (int i = 1; i <= angle;i++){
        usleep(100'000);
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
            angle = 142;
        break;
        case 1:
            angle = 42;
        break;
        case 2:
            angle = 152;
        break;
    }
    std::cout << pince << " " << angle << std::endl;
    this->pwm_setServoPosition(pince, angle);
}

void MyTCPClient::ouvrir_pince(int pince) {
    int angle;
    if (pince < 0 || pince > 2){
        return;
    }
    switch(pince){
        case 0:
            angle = 115;
        break;
        case 1:
            angle = 22;
        break;
        case 2:
            angle = 125;
        break;
    }
    std::cout << pince << " " << angle << std::endl;
    this->pwm_setServoPosition(pince, angle);
}


void MyTCPClient::check_panneau(int quelBras) {
    this->pwm_setServoPosition(quelBras, 30);
}

void MyTCPClient::uncheck_panneau(int quelBras) {
    this->pwm_setServoPosition(quelBras, 0);
}
