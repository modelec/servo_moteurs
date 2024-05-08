#include "MyTCPClient.h"
#include <atomic>
#include <csignal>

std::atomic<bool> shouldStop = false;

void signalHandler( int signum ) {
    shouldStop = true;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    int port = 8080;
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

    try{
        client.start();

        client.sendMessage("servo_moteur;strat;ready;1");

        while(!client.shouldStop() && !shouldStop){
            usleep(100'000);
        }

    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}