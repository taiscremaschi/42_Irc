
#include "Irc.hpp"

int main() {
    // Criando o socket
    int myFirstSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (myFirstSocket == -1) {
        std::cerr << "Erro ao criar o socket\n";
        return 1;
    }

    // Configurando o endereço do servidor
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Porta 8080
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Conectando ao servidor
    if (connect(myFirstSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erro ao conectar ao servidor\n";
        return 1;
    }

    std::cout << "Conectado ao servidor\n";

    // Aqui você pode enviar e receber dados do servidor

    // Fechando o socket
    close(myFirstSocket);

    return 0;
}