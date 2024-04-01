#include "MyTCPClient.h"

MyTCPClient::MyTCPClient(const char *serverIP, int port) : TCPClient(serverIP, port) {
    this->handle = -1;
}

int MyTCPClient::init() {
    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "Impossible d'initialiser pigpio\n");
        return 1;
    }

    this->handle = i2cOpen(1, PCA9685_ADDR, 0);

    if (this->handle < 0)
    {
        fprintf(stderr, "Impossible d'ouvrir la connexion I2C\n");
        gpioTerminate();
        return 1;
    }

    initPCA9685(this->handle);

    return 0;
}

void MyTCPClient::handleMessage(const std::string &message) {
    std::vector<std::string> token = TCPSocket::split(message, ";");

    if (token.size() != 4) {
        std::cerr << "Message invalide: " << message << std::endl;
        return;
    }

    if (token[1] == "servo_pot" || token[1] == "all") {
        if (token[2] == "ping") {
            this->sendMessage("pong");
        }
        else if (token[2] == "ouvrir pince") {
            int pince = std::stoi(token[3]);
            ouvrir_pince(handle, pince);
        }
        else if (token[2] == "fermer pince") {
            int pince = std::stoi(token[3]);
            fermer_pince(handle, pince);
        }
        else if (token[2] == "baisser bras") {
            baisser_bras(handle);
        }
        else if (token[2] == "lever bras") {
            lever_bras(handle);
        }
    }
}

MyTCPClient::~MyTCPClient() {
    i2cClose(this->handle);

    // Terminaison de pigpio
    gpioTerminate();
}
