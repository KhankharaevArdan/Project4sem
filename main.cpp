#include <iostream>
#include <string>

#include "Game.hpp"
#include "NetworkServer.hpp"
#include "NetworkClient.hpp"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Incorrect type of Server/Client" << std::endl;
        return 0;

    } else {
        if( static_cast<std::string>(argv[1]) == "Server" ) {
            NetWorkServer server;
            std::cout << "Server's IpAddress: ";
            std::cout << server.getIp() << std::endl;
            server.Init();
            server.RegisterNewClients();
            server.ReceiveAndSendData();
         } else if( static_cast<std::string>(argv[1]) == "Client" ) {
            NetWorkClient client;
            client.Init();
            client.RegisterOnServer();
            PixelGameEngine Game{};
            Game.StartGame(client);
        }
    }
    
    return 0;
}