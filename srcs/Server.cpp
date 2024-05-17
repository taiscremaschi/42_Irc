
#include "Server.hpp"

Server::Server() { 
    std::cout << "this is server constructor" << std::endl;
}

Server::~Server() {
    std::cout << "this is server destructor" << std::endl;
}

int Server::_myPortConvertor(char *av)
{
    int i = 0;
    int result = 0;
    while(av[i])
    {
        if(av[i] < 48 || av[i] > 57)
            return -1;
        i++;
    }
    if(i  > 5)
        return -1;
    i = 0;
    while(av[i])
    {
        result = result * 10; 
        result = result + av[i] - '0';
        i++;
    }
    if(result < 1024 || result > 65535)
        return -1;

    return result;
}

int Server::setPort(char *av){

    _port  = _myPortConvertor(av);
    return (_port);
}

void Server::inicializeServer() {
    
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0); 

    if(_serverSocket == -1){
        std::cerr << "error in creating server socket\n";
    }
    sockaddr_in serverAddr; // now I need configure the server address (read help 2)
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(_port); 

    if(bind(_serverSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        close(_serverSocket);
        std::cerr << "error in bind" << std::endl; //help 3
        return;
    }
    if(listen(_serverSocket, 5) == -1) {
        std::cerr << "error in listen\n"; // max_nbr of connections
        close(_serverSocket);
        return;

    }
    std::cout << "Server waiting for connections...\n";
    
    pollfd server;
    server.fd = _serverSocket;
    server.events = POLLIN; 
    _fds.push_back(server); //add the server socket to the set of descriptors to be monitored by poll()

    while (1) {
        int result = poll(_fds.data(), _fds.size(), -1); // Espera indefinidamente por eventos
        if (result < 0) {
            std::cerr << "Error in poll()\n";
            break;
        }
        for (size_t i = 0; i < _fds.size(); ++i) 
        {
            if (_fds[i].revents & POLLIN) {
                if (_fds[i].fd == _serverSocket) 
                {
                    // Novo cliente tentando se conectar
                    sockaddr_in clientAddr;
                    socklen_t clientAddrSize = sizeof(clientAddr);
                    int clientSocket = accept(_serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
                    if (clientSocket == -1) 
                    {
                        std::cerr << "Erro in accept conection\n";
                        continue;
                    }
                    pollfd client;
                    client.fd = clientSocket;
                    client.events = POLLIN;
                    _fds.push_back(client);
                    Client newClient(clientSocket);
                    _clients.push_back(newClient);
                    std::cout << "new client conected\n";
                } 
                else 
                {
                    // Cliente existente enviando dados
                    char buffer[1024];
                    int bytesReceived = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytesReceived <= 0) { 
                        // Erro ou conexão fechada pelo cliente
                        if (bytesReceived == 0) {
                            std::cout << "Client disconnected\n";
                        } 
                        else {
                            std::cerr << "Error in recv\n";
                        }
                        close(_fds[i].fd);
                        _fds.erase(_fds.begin() + i);
                        _clients.erase(_clients.begin() + i);
                        continue;
                    }
                    buffer[bytesReceived] = '\0';
                    std::cout << "Received: " << buffer << std::endl;
                }
            }
        }
    }
    close(_serverSocket);
}


void Server::savePass(char *av)
{
    _password = av;

}