
#include "Irc.hpp"

int main(int ac, char **av)
{

    if(ac != 3)
        return (errorMsg("Wrong number of arguments"));
    int port = myPortConvertor(av[1]);
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
    
    while(1)
    {





    }
    std::cout << "Servidor aguardando por conexões...\n";

   // Aceitando conexões
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(myFirtSocket, (sockaddr*)&clientAddress, &clientAddressSize);
    
    if (clientSocket == -1) {
        std::cerr << "Erro ao aceitar a conexão\n";
        return 1;
    }
     std::cout << "Cliente conectado\n";
    
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-termina a string recebida
        std::cout << "Recebido: " << buffer << std::endl;
    }


    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-termina a string recebida
        std::cout << "Recebido: " << buffer << std::endl;
    }

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-termina a string recebida
        std::cout << "Recebido: " << buffer << std::endl;
    }



    const char* message = "Bem-vindo ao servidor!\r\n";
    send(clientSocket, message, strlen(message), 0);



    close(clientSocket);
    close(myFirtSocket);
    return 0;
}