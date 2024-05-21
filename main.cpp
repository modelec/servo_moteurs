#include "MyTCPClient.h"
#include <atomic>
#include <csignal>
#include <Modelec/CLParser.h>

std::atomic<bool> shouldStop = false;

void signalHandler( int signum ) {
    shouldStop = true;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    CLParser clParser(argc, argv);

    int port = clParser.getOption<int>("port", 8080);

    auto host = clParser.getOption("host", "127.0.0.1");

    MyTCPClient client(host, port);

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