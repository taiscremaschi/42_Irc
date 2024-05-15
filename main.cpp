
#include "Irc.hpp"

int main(int ac, char **av)
{

    if(ac != 3)
        return (errorMsg("Wrong number of arguments"));
    int port = myPortConvertor(av[1]);
    if(port == -1)
        return (errorMsg("Invalid port number"));

    // TODO: tratar das passwords

    int myFirtSocket = socket(AF_INET, SOCK_STREAM, 0); //my notes 1
    if(myFirtSocket == -1){
        std::cerr << "error in create server socket\n";
        return 1;
    }
    sockaddr_in serverAddr; // now I need configure the server address (read help 2)
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(port); 

    if(bind(myFirtSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "erro ao fazer bind" << std::endl; //help 3
    }
    if(listen(myFirtSocket, 5) == -1) {
        std::cerr << "erro ao fazer listen\n"; // max_nbr of connections
    }
    std::cout << "Servidor aguardando por conexões...\n";

    std::vector<ClientInfo> clients;
    clients.reserve(10); // Reserva espaço para 10 clientes

    std::vector<pollfd> fds;
    pollfd server;
    server.fd = myFirtSocket;
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
                if (fds[i].fd == myFirtSocket) {
                    // mNovo cliente tentando se conectar
                    sockaddr_in clientAddr;
                    socklen_t clientAddrSize = sizeof(clientAddr);
                    int clientSocket = accept(myFirtSocket, (sockaddr*)&clientAddr, &clientAddrSize);
                    if (clientSocket == -1) {
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
                } else {
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
    close(myFirtSocket);
    return 0;
}
