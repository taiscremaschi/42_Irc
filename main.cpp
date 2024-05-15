
#include "Irc.hpp"

int main(int ac, char **av)
{

    if(ac != 3)
        return (errorMsg("Wrong number of arguments"));
    int port = myPortConvertor(av[1]);
    printf("%d\n", port);
    if(port == -1)
        return (errorMsg("Invalid port number"));

    // TODO: tratar das passwords



    //crio uma variavel inteira com o socket e os parametros
    //af_net é o dominio e sera usando o protocoloco ipv4 
    //o sock_stream iéo type indicando uma conexao tcp ou sseja
    //os dados serao trasmitidos de forma sequencial sem eroros erros
    //ou duplicatas. ja o ultimo eh o protocolo que 0 indica que o sistam vai escolher 
    //o protocolo apropriado automaticamente basendo no domonio e no tipo especificado

    int myFirtSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(myFirtSocket == -1){
        std::cerr << "deu ruim\n";
        return 1;
    }
    //agr preciso configurar o endereco do servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; //
    serverAddr.sin_port = htons(port); //porta padrao do irc 6667

    //afr ligar o o socket ao endereço 
    //A função bind() é utilizada para associar um endereço IP e uma porta a um socket.
    // Isso é necessário para que o sistema operacional saiba onde direcionar os dados que chegam para essa porta específica.
    if(bind(myFirtSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "erro ao fazer bind" << std::endl;
    }
    //listen pro socket com o numero max de conexoes pendentes que o sisteman
    //pode infileirar antes de começar a rejeitar novas conexoes
    if(listen(myFirtSocket, 5) == -1) {
        std::cerr << "erro ao fazer listen\n";
    }
  
    std::cout << "Servidor aguardando por conexões...\n";

    std::vector<ClientInfo> clients;
    clients.reserve(10); // Reserva espaço para 10 clientes

    // Adicione o socket do servidor ao conjunto de descritores a serem monitorados por poll()
    std::vector<pollfd> fds;
    pollfd server;
    server.fd = myFirtSocket;
    server.events = POLLIN;
    fds.push_back(server);

    while (true) {
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

    // Fechar sockets, limpar recursos, etc...
    close(myFirtSocket);
    return 0;
}
