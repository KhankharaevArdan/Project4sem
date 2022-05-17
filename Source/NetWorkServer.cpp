#include <NetworkServer.hpp>

NetWorkServer::NetWorkServer() {
    ip_ = sf::IpAddress::getLocalAddress();
}

sf::IpAddress NetWorkServer::getIp() const {
    return ip_;
}

sf::Socket::Status NetWorkServer::Init() {
    if(listener_.listen(sf::Socket::AnyPort) == sf::Socket::Status::Done) {
        std::cout << "Server's Port: ";
        std::cout << listener_.getLocalPort() << std::endl;
        std::cout << "Waiting Clients" << std::endl;
         return sf::Socket::Status::Done;
    }else {
        return sf::Socket::Status::Error;
    }
}

sf::Socket::Status NetWorkServer::RegisterNewClients() {
    
    // std::string buff;
    // packet_.clear();
    // clients_.at(0).data_socket->receive(packet_, clients_.at(0).ip, clients_.at(0).port);
    // packet_ >> buff;
    // std::cout << buff << std::endl;
    // packet_.clear();
    ReceiveClientsRegData();
    packet_.clear();
    packet_ << clients_.at(1).port << clients_.at(1).ip.toString();
    clients_.at(0).data_socket->send(packet_, clients_.at(0).ip, clients_.at(0).port);

    packet_.clear();
    packet_ << clients_.at(0).port << clients_.at(0).ip.toString();
    clients_.at(1).data_socket->send(packet_, clients_.at(1).ip, clients_.at(1).port);
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkServer::AcceptNewConnections() {
    if(listener_.accept(reg_socket_) != sf::Socket::Status::Done) {
        std::cout << "Failure to accept" << std::endl;
        return sf::Socket::Status::Error;
    }else {
        std::cout << "New client connected: " << reg_socket_.getRemoteAddress() << std::endl;
        return sf::Socket::Status::Done;
    }
}

sf::Socket::Status NetWorkServer::ReceiveClientsRegData() {
    //if(reg_socket_.isBlocking()) reg_socket_.setBlocking(false);
    int i = 0;
    while( i < max_quan_clients) {
        AcceptNewConnections();
        if(reg_socket_.receive(packet_) == sf::Socket::Status::Done) {
            if(packet_.getDataSize() > 0) {
                Client new_client;
                packet_ >> new_client.name;
                packet_ >> new_client.port;
                new_client.ip = reg_socket_.getRemoteAddress();
                new_client.data_socket = new sf::UdpSocket;
                new_client.data_socket->bind(sf::Socket::AnyPort);
                //std::cout << new_client.data_socket->getLocalPort() << std::endl;
                ++i;
                clients_.push_back(new_client);
                SendServerUdpPort(new_client);
                reg_socket_.disconnect();
                packet_.clear();
            }else {
                std::cout << "Packet is empty" << std::endl;
                return sf::Socket::Status::Error; 
            }
        }else {
            std::cout << "Failure to receive packet" << std::endl;
            return sf::Socket::Status::Error;
        }
    }
    packet_.clear();
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkServer::SendServerUdpPort(const Client& client) {
    packet_.clear();
    std::string tmp = "Dear, ";
    tmp += client.name;
    tmp += ". You successfully connected by UDP";
    packet_ << tmp;
    if(client.data_socket->send(packet_, client.ip, client.port) != sf::Socket::Status::Done) {
        std::cout << "Error in sending udp port" << std::endl;
        return sf::Socket::Status::Error;
    }
    packet_.clear();
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkServer::ReceiveAndSendData() {
    clients_.at(0).data_socket->setBlocking(false);
    clients_.at(1).data_socket->setBlocking(false);
    std::cout << "Sendint And Receiving" << std::endl;
    while(true) {
        packet_.clear();
        if(clients_.at(0).data_socket->receive(packet_, clients_.at(0).ip, clients_.at(0).port) == sf::Socket::Status::Done) {
            clients_.at(1).data_socket->send(packet_, clients_.at(1).ip, clients_.at(1).port);
            packet_.clear();
            std::cout << "here3" << std::endl;
        }else if(clients_.at(1).data_socket->receive(packet_, clients_.at(1).ip, clients_.at(1).port) == sf::Socket::Status::Done) {
            clients_.at(0).data_socket->send(packet_, clients_.at(0).ip, clients_.at(0).port);
            packet_.clear();
            std::cout << "here2" << std::endl;
        }
        else {
            std::cout << "Can't Receive and Send" << std::endl;
            //eturn sf::Socket::Status::Error;
        }
    }
    
}

NetWorkServer::~NetWorkServer() {
    for(auto client : clients_) {
        delete client.data_socket;
    }
}