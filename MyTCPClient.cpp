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
            this->fermer_pince(0);
            this->fermer_pince(2);
            this->baisser_bras();
        }
        else if(token[2] == "transport bras"){
            this->fermer_pince(0);
	    this->fermer_pince(2);
	    this->transport_bras();
        } 
         else if (token[2] == "lever bras") {
            this->fermer_pince(0);
            this->fermer_pince(2);
            this->lever_bras();
        }
        else if (token[2] == "check panneau") {
            int bras = std::stoi(token[3]);
            this->check_panneau(bras);
        }
        else if (token[2] == "uncheck panneau") {
            int bras = std::stoi(token[3]);
            this->uncheck_panneau(bras);
        } else if (token[2] == "angle") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");
            if (args.size() != 2) {
                std::cerr << "Nombre d'arguments invalide" << std::endl;
                return;
            }
            int servo = std::stoi(args[0]);
            int angleDeg = static_cast<int>(std::stof(args[1]));
            this->pwm_setServoPosition(servo, angleDeg);
        } else if (token[2] == "clear") {
            this->pwm_clear();
        }
    }
}

void MyTCPClient::pwm_setFrequency(float freq) {
    pca.set_pwm_freq(freq);
}

void MyTCPClient::pwm_init() {
    pwm_setFrequency(50.0);
    this->fermer_pince(0, true);
    this->fermer_pince(1, true);
    this->fermer_pince(2, true);
    this->baisser_bras();//this->baisser_bras(true);
}


void MyTCPClient::pwm_setServoPosition(int servo, int position) {
    int on_time = SERVO_MIN + (SERVO_MAX - SERVO_MIN) * position / 180 - 1;//temps ou le servo est allumé par rapport à 4096
    pca.set_pwm(servo, 0, on_time);
}

void MyTCPClient::baisser_bras() {
    int angleDebut;
    switch(this->positionBras){
        case BRAS_HAUT:
            angleDebut = angleBrasHaut.servo4;
            break;
        case BRAS_TRANSPORT:
            angleDebut = angleBrasTransport.servo4;
            break;
        case BRAS_BAS:
            this->pwm_setServoPosition(4, angleBrasBas.servo4);
            this->pwm_setServoPosition(5, angleBrasBas.servo5);
            return;
    }
    for(int i = angleDebut; i <= angleBrasBas.servo4;i++){
        this->pwm_setServoPosition(4, i);
        usleep(5'000);
        this->pwm_setServoPosition(5, 180-i);
    }
    this->positionBras = BRAS_BAS;
    this->pwm_setServoPosition(4, angleBrasBas.servo4);
    this->pwm_setServoPosition(5, angleBrasBas.servo5);
}

void MyTCPClient::transport_bras(){
    switch(this->positionBras){
        case BRAS_BAS:
            for (int i = angleBrasBas.servo5; i <= angleBrasTransport.servo5; i++){
                this->pwm_setServoPosition(4, 180-i);
                usleep(5'000);
                this->pwm_setServoPosition(5, i);
            }
            break;
        case BRAS_HAUT:
            for (int i = angleBrasHaut.servo4; i <= angleBrasTransport.servo4; i++){
                this->pwm_setServoPosition(4, i);
                usleep(5'000);
                this->pwm_setServoPosition(5, 180-i);
            }
            break;
        case BRAS_TRANSPORT:
            this->pwm_setServoPosition(4, angleBrasTransport.servo4);
            this->pwm_setServoPosition(5, angleBrasTransport.servo5);
            return;
    }
    this->positionBras = BRAS_TRANSPORT;
    this->pwm_setServoPosition(4, angleBrasTransport.servo4);
    this->pwm_setServoPosition(5, angleBrasTransport.servo5);
}

void MyTCPClient::lever_bras() {
    int angleDebut;
    switch(this->positionBras){
        case BRAS_BAS:
            angleDebut = angleBrasBas.servo5;
            break;
        case BRAS_TRANSPORT:
            angleDebut = angleBrasTransport.servo5;
            break;
        case BRAS_HAUT:
            this->pwm_setServoPosition(4, angleBrasHaut.servo4);
            this->pwm_setServoPosition(5, angleBrasHaut.servo5);
            return;
    }
    for (int i = angleDebut;i <= angleBrasHaut.servo5;i++){
        this->pwm_setServoPosition(4, 180-i);
        usleep(5'000);
        this->pwm_setServoPosition(5, i);
    }
    this->positionBras = BRAS_HAUT;
    this->pwm_setServoPosition(4, angleBrasHaut.servo4);
    this->pwm_setServoPosition(5, angleBrasHaut.servo5);
}

void MyTCPClient::fermer_pince(int pince, bool force) {
    anglePince pinceChoisie;
    int angleDebut;
    if ((etatPince[pince] == PINCE_FERMER && !force) || pince < 0 || pince > 2){
        return;
    }
    switch(pince){
        case 0:
            pinceChoisie = anglePince0;
            break;
        case 1:
            pinceChoisie = anglePince1;
            break;
        case 2:
            pinceChoisie = anglePince2;
            break;
    }
    switch (etatPince[pince]) {
        case PINCE_FERMER:
            this->pwm_setServoPosition(pince, pinceChoisie.fermer);
            return;
        case PINCE_MIDDLE:
            angleDebut = pinceChoisie.middle;
            break;
        case PINCE_OUVERTE:
            angleDebut = pinceChoisie.ouverte;
            break;
    }
    std::cout << "Fermer pince : " << pince << std::endl;
    for(int i = angleDebut; i <= pinceChoisie.fermer;i++){
        this->pwm_setServoPosition(pince, i);
        usleep(5'000);
    }
    etatPince[pince] = PINCE_FERMER;
}

void MyTCPClient::middle_pince(int pince, bool force){
    anglePince pinceChoisie;
    if ((etatPince[pince] == PINCE_MIDDLE && !force) || pince < 0 || pince > 2){
        return;
    }
    switch(pince){
        case 0:
            pinceChoisie = anglePince0;
            break;
        case 1:
            pinceChoisie = anglePince1;
            break;
        case 2:
            pinceChoisie = anglePince2;
            break;
    }
    std::cout << "Middle pince : " << pince << std::endl;
    switch (etatPince[pince]) {
        case PINCE_OUVERTE:
            for(int i = pinceChoisie.ouverte; i <= pinceChoisie.middle;i++){
                this->pwm_setServoPosition(pince, i);
                usleep(5'000);
            }
            break;
        case PINCE_MIDDLE:
            this->pwm_setServoPosition(pince, pinceChoisie.middle);
            return;
        case PINCE_FERMER:
            for (int i = pinceChoisie.fermer; i >= pinceChoisie.middle;i--){
                this->pwm_setServoPosition(pince, i);
                usleep(5'000);
            }
            break;
    }
    etatPince[pince] = PINCE_OUVERTE;
}

void MyTCPClient::ouvrir_pince(int pince, bool force) {
    anglePince pinceChoisie;
    int angleDebut;
    if ((etatPince[pince] == PINCE_OUVERTE && !force) || pince < 0 || pince > 2){
        return;
    }
    switch(pince){
        case 0:
            pinceChoisie = anglePince0;
            break;
        case 1:
            pinceChoisie = anglePince1;
            break;
        case 2:
            pinceChoisie = anglePince2;
            break;
    }
    switch (etatPince[pince]) {
        case PINCE_FERMER:
            angleDebut = pinceChoisie.fermer;
            break;
        case PINCE_MIDDLE:
            angleDebut = pinceChoisie.middle;
            break;
        case PINCE_OUVERTE:
            this->pwm_setServoPosition(pince, pinceChoisie.ouverte);
            return;
    }
    std::cout << "Ouvrir pince : " << pince << std::endl;
    for (int i = angleDebut; i >= pinceChoisie.ouverte;i--){
        this->pwm_setServoPosition(pince, i);
        usleep(5'000);
    }
    etatPince[pince] = PINCE_OUVERTE;
}


void MyTCPClient::check_panneau(int quelBras) {
    for (int i = PANO_BAS; i <= PANO_HAUT;i++){
        this->pwm_setServoPosition(quelBras, i);
    }
}

void MyTCPClient::uncheck_panneau(int quelBras) {
    for (int i = PANO_HAUT; i >= PANO_BAS;i--){
        this->pwm_setServoPosition(quelBras, i);
    }
}

void MyTCPClient::pwm_clear() {
    pca.set_all_pwm(0,0);
}
