
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
        std::cerr << "error in create server socket\n";
    }
    sockaddr_in serverAddr; // now I need configure the server address (read help 2)
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(_port); 

    if(bind(_serverSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "erro ao fazer bind" << std::endl; //help 3
    }
    if(listen(_serverSocket, 5) == -1) {
        std::cerr << "erro ao fazer listen\n"; // max_nbr of connections
    }
    std::cout << "Servidor aguardando por conexões...\n";
    
    std::vector<ClientInfo> clients;

    clients.reserve(10); // Reserva espaço para 10 clientes

    std::vector<pollfd> fds;
    pollfd server;
    server.fd = _serverSocket;
    server.events = POLLIN; 
    fds.push_back(server); //add the server socket to the set of descriptors to be monitored by poll()

    while (1) {
        int ret = poll(fds.data(), fds.size(), -1); // Espera indefinidamente por eventos
        if (ret < 0) {
            std::cerr << "Erro em poll()\n";
            break;
        }
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _serverSocket) 
                {
                    // Novo cliente tentando se conectar
                    sockaddr_in clientAddr;
                    socklen_t clientAddrSize = sizeof(clientAddr);
                    int clientSocket = accept(_serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
                    if (clientSocket == -1) 
                    {
                        std::cerr << "Erro ao aceitar a conexão\n";
                        continue;
                    }
                    pollfd client;
                    client.fd = clientSocket;
                    client.events = POLLIN;
                    fds.push_back(client);
                    ClientInfo tata;
                    tata.socket = clientSocket;
                    clients.push_back(tata);
                    std::cout << "Novo cliente conectado\n";
                } 
                else 
                {
                    // Cliente existente enviando dados
                    char buffer[1024];
                    int bytesReceived = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytesReceived <= 0) { 
                        // Erro ou conexão fechada pelo cliente
                        if (bytesReceived == 0) {
                            std::cout << "Cliente desconectado\n";
                        } else {
                            std::cerr << "Erro ao receber dados do cliente\n";
                        }
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        clients.erase(clients.begin() + i);
                        continue;
                    }
                    buffer[bytesReceived] = '\0';
                    std::cout << "Recebido: " << buffer << std::endl;
                }
            }
        }
    }
    close(_serverSocket);
}
