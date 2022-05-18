#include <NetworkClient.hpp>

sf::Socket::Status NetWorkClient::Init(unsigned short preferablePort) {
    std::cout << "Enter Server's IpAddress" << std::endl;
    std::string tmp;
    std::getline(std::cin, tmp);

    server_ip_ = sf::IpAddress(tmp);
    
    std::cout << "Enter Server's Port" << std::endl;
    std::cin >> reg_port_;

    sf::Socket::Status status = reg_socket_.connect(server_ip_, reg_port_);
    if(status != sf::Socket::Status::Done) {
        std::cout << "Failure to connect" << std::endl;
        return status;
    }

    tmp.clear();
    std::cout << "Enter Your Name" << std::endl;
    std::cin >> name_;

    if(data_socket_.bind(preferablePort) == sf::Socket::Status::Done) {
        std::cout << "Successfully binded to port:" << data_socket_.getLocalPort() << std::endl;
    }else {
        std::cout << "Failure to bind" << std::endl;
        return sf::Socket::Status::Error;
    }
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkClient::RegisterOnServer() {
    SendRegisterClientData();
    ReceiveServerUdpPort();
    return ReceiveOpponentUdpPort();
}

sf::Socket::Status NetWorkClient::SendRegisterClientData() {
    if(!reg_socket_.isBlocking()) reg_socket_.setBlocking(true);

    sf::Packet packet;
    packet << name_ << data_socket_.getLocalPort();
    if(reg_socket_.send(packet) == sf::Socket::Status::Done) {
        std::cout << "SendRegisterClientData() : Successfully" << std::endl;
    }else {
        //std::cout << "Failed to send client data" << std::endl;
        return sf::Socket::Status::Error;
    }
    packet_.clear();
    
}

sf::Socket::Status NetWorkClient::ReceiveServerUdpPort() {
    data_socket_.setBlocking(true);
    if(data_socket_.receive(packet_, server_ip_, server_udp_port_) == sf::Socket::Status::Done) {
        std::string tmp;
        packet_ >> tmp;
        std::cout << tmp << std::endl;
        //std::cout << server_udp_port_ << std::endl;
        // packet_.clear();
        // tmp.clear();
        // tmp += "THX";
        // packet_ << tmp;
        // data_socket_.send(packet_, server_ip_, server_udp_port_);
    }else {
        std::cout << "Failure to Receive Udp Port" << std::endl;
        return sf::Socket::Status::Error;
    }
    packet_.clear();
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkClient::ReceiveOpponentUdpPort() {
    data_socket_.setBlocking(true);
    if(data_socket_.receive(packet_, server_ip_, server_udp_port_) == sf::Socket::Status::Done) {
        std::string tmp;
        packet_ >> opponent_udp_port_ >> tmp;
        opponent_ip_ = sf::IpAddress(tmp);
        std::cout << opponent_udp_port_ << std::endl;
        std::cout << opponent_ip_ << std::endl;
    }else {
        std::cout << "Failure to Receive Udp Port" << std::endl;
        return sf::Socket::Status::Error;
    }
    packet_.clear();
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkClient::SendData(const Hero& tank) {
    packet_.clear();
    auto coord = tank.GetCoord();
    packet_ << coord.first << coord.second;
    data_socket_.setBlocking(true);
    std::string dir;
    switch (tank.GetDirection())
    {
        case Direction::UP :
        dir = "UP";
        break;
    
        case Direction::RIGHT :
        dir = "RIGHT";
        break;
    
        case Direction::LEFT :
        dir = "LEFT";
        break;
    
        case Direction::DOWN :
        dir = "Down";
        break;

        default:
        break;
    }
    packet_ << dir;
    data_socket_.send(packet_, server_ip_, server_udp_port_);
    packet_.clear();
    //std::cout << "here0" << std::endl;
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkClient::ReceiveData(Hero& tank_friend) {
    data_socket_.setBlocking(true);
    if(data_socket_.receive(packet_, server_ip_, server_udp_port_) == sf::Socket::Status::Done) {
        std::pair<int, int> coord;
        std::string dir;
        packet_ >> coord.first >> coord.second >> dir;

        tank_friend.SetPosition(coord);
        if(dir == "UP") {
            tank_friend.ChangeDirection(Direction::UP);
        }else if(dir == "DOWN") {
            tank_friend.ChangeDirection(Direction::DOWN);
        }else if(dir == "RIGHT") {
            tank_friend.ChangeDirection(Direction::RIGHT);
        }else if(dir == "LEFT") {
            tank_friend.ChangeDirection(Direction::LEFT);
        }
        std::cout << "here1" << std::endl;
    }
    
    packet_.clear();
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkClient::SendDataToOpponent(const Hero& tank) {
    packet_.clear();
    auto coord = tank.GetCoord();
    packet_ << coord.first << coord.second;
    data_socket_.setBlocking(true);
    std::string dir;
    switch (tank.GetDirection())
    {
        case Direction::UP :
        dir = "UP";
        break;
    
        case Direction::RIGHT :
        dir = "RIGHT";
        break;
    
        case Direction::LEFT :
        dir = "LEFT";
        break;
    
        case Direction::DOWN :
        dir = "Down";
        break;

        default:
        break;
    }
    packet_ << dir;
    data_socket_.send(packet_, opponent_ip_, opponent_udp_port_);
    packet_.clear();
    return sf::Socket::Status::Done;
}

sf::Socket::Status NetWorkClient::ReceiveDataFromOpponent(Hero& tank_friend) {
    data_socket_.setBlocking(true);
    packet_.clear();
    if(data_socket_.receive(packet_, opponent_ip_ , opponent_udp_port_) == sf::Socket::Status::Done) {
        std::pair<int, int> coord;
        std::string dir;
        packet_ >> coord.first >> coord.second >> dir;

        tank_friend.SetPosition(coord);
        if(dir == "UP") {
            tank_friend.ChangeDirection(Direction::UP);
        }else if(dir == "DOWN") {
            tank_friend.ChangeDirection(Direction::DOWN);
        }else if(dir == "RIGHT") {
            tank_friend.ChangeDirection(Direction::RIGHT);
        }else if(dir == "LEFT") {
            tank_friend.ChangeDirection(Direction::LEFT);
        }
        //std::cout << "here1" << std::endl;
    }
    
    packet_.clear();
    return sf::Socket::Status::Done;
}