#include "MyTCPClient.h"

int main(int argc, char* argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

    client.start();

    client.sendMessage("servo_moteur;strat;ready;1");

    while (!client.shouldStop()) {
        usleep(100'000);
    }

    return 0;
}