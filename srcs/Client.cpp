
#include "Client.hpp"

Client::Client(int clientSocket) {
   _socketClient =  clientSocket;
}

Client:: ~Client(){
}

int Client::getSocketClient(){
    return _socketClient;
}

void Client::setSocketClient(int socketClient){
    _socketClient = socketClient;
}

std::string Client::getName() const{
    return _name;
}


std::string Client::getNickname() const{
    return _nickname;
}

void Client::setName(std::string name){
    _name = name;
}

void Client::setNickname(std::string nick){
    _nickname = nick;
}