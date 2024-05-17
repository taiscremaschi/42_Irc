
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


//salvar o nick 
//se receber um nick salvar o nome novo
//depois mandar pro servidor
//funcao send


