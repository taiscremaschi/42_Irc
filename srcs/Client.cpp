
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
