#include "MyTCPClient.h"

int main(int argc, char* argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

    client.start();

    client.sendMessage("servo_pot;strat;ready;1");

    while (true) {
        std::string message;
        std::cout << "Enter message ('quit' to exit): ";
        std::getline(std::cin, message);

        if (message == "quit") {
            client.stop();
            break;
        }
        if (message == "clear") {
            client.pwm_clear();
        }
        else if (message == "close") {
            client.fermer_pince(0);
            client.fermer_pince(1);
            client.fermer_pince(2);
        }

        client.sendMessage(message.c_str());
    }

    return 0;
}