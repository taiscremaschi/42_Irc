
#include "Server.hpp"
#include "IrcMessages.hpp"

Server::Server() {}

Server::~Server() {}

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

void Server::savePass(char *av)
{
    _password = av;
}

std::string     Server::readData(int i){
    
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
        return std::string();
    }
    buffer[bytesReceived] = '\0';
    std::string result = buffer;
    return result;
  
}

void Server::newClientConnection()
{
    // Novo cliente tentando se conectar
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = accept(_serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == -1) 
    {
        std::cerr << "Erro in accept conection\n";
        return;
    }
    pollfd client;
    client.fd = clientSocket;
    client.events = POLLIN;
    _fds.push_back(client);
    Client newClient(clientSocket);
    _clients.push_back(newClient);
    std::cout << "new client conected\n";

}

void MsgforHex(int clientSocket, const std::string& message) 
{
    std::string msg = message + "\r\n";
    std::cout << msg << std::endl;
    send(clientSocket, msg.c_str(), msg.length(), 0); //funcao para mandar mensagem pra outro socket
}

std::string Server::channelExists(Client& client, const std::string& channelName){
    for(size_t i = 0; i < _channels.size(); ++i)
    {
        if(_channels[i].getName() == channelName)
        {
            //TODO: ADC O @ APENAS AO OPERADOR
            std::string namesList = ":servidor 353 " + client.getNickname() + " = " + channelName + " :";
           // std::cerr << "nameliste é   " << namesList << std::endl;

            _channels[i].addClient(client);
            std::vector<std::string> result = _channels[i].getAllClientsName();
            for(size_t j = 0; j < result.size(); ++j)
                    namesList += result[j] + " ";
           // std::cerr << "entrou   " << namesList << std::endl;
            //std::cerr << "entrou   " << channelName  << std::endl;
            return namesList;
        }
    }
    //std::cerr << "nao entrou   " << channelName  << std::endl;
    return "";
}

// void        Server::printChannels()
// {
//     std::cout << "list de channels:" << std::endl;

//     for (Channel& channel : _channels)
//     {
        
//         std::cout << "nome do channels: " <<  channel.getName() << std::endl;
//         std::vector<std::string> users = channel.getAllClientsName();
//         std::cout << "lista de utilizadores: " << std::endl;
        
//         for (std::string& user : users)
//         {
//             std::cout << "\t - " << user << std::endl;
//         }
//         std::cout << "\n";
//     }
// }


void Server::handleJoinCommand(Client& client, const std::string& channelName) 
{

    // std::cout << "antes do chanel exist meu vec + 1 is   " << channelName  << std::endl;
    std::string namesMessage  = channelExists(client, channelName);
    if(namesMessage.empty()){
        Channel newchannel(channelName, client);
        _channels.push_back(newchannel);
        //std::cout << "meu novo chanel " << newchannel.getName() << std::endl;
        namesMessage = ":servidor 353 " + client.getNickname() + " = " + channelName + " :@" + client.getNickname();
    }
    std::string joinMsg = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName;
    std::string topicMsg = ":servidor 332 " + client.getNickname() + " " + channelName + " :Tópico inicial do canal";

    std::string topicCreatorMsg = ":servidor 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " 0";
    std::string endOfNameMsg = ":servidor 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.";


    //printChannels();

    MsgforHex(client.getSocketClient(), joinMsg);
    MsgforHex(client.getSocketClient(), topicMsg);
    MsgforHex(client.getSocketClient(), topicCreatorMsg);
    MsgforHex(client.getSocketClient(), namesMessage);
    MsgforHex(client.getSocketClient(), endOfNameMsg);

}

void Server::infoForChannel(Client &client, std::string channelName)
{
    //std::cout << "entrou \n" << std::endl;
    size_t i =  0;
    for(; i < _channels.size(); ++i)
    {
        //std::cout << _channels[i].getName() << " this is my name." << std::endl;
        if(_channels[i].getName() == channelName)
            break;
    }
    if (i == _channels.size())
        return ;
    std::vector<Client>  clients = _channels[i].getAllClients();
    std:: string msg = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName; 
    for (size_t i = 0; i < clients.size(); ++i)
    {
        //std::cout << "devia entrar nesse for " << clients[i].getNickname() << std::endl;
        if(clients[i].getNickname() != client.getNickname())
        {
           // std::cout << clients[i].getNickname() << "devia entrar" << std::endl;

            MsgforHex(clients[i].getSocketClient(), msg);
        }
    }
}

void Server::findCmd(const std::vector<std::string> &vec, Client &client, int clientSocket) {
    
    for (size_t i = 0; i < vec.size(); ++i) {
        // if (vec[i] == "USER") { //isso aqui ta erradao
        //     client->setName(vec[i + 1]);
        //     std::cout << "User set to: " << vec[i + 1] << std::endl;
        //     std::string response = "USER " + vec[i + 1] + " 0 * :realname";
        //     MsgforHex(clientSocket, response);
        // }
        if (vec[i] == "NICK") 
        {
            for(size_t x = 0; x < _clients.size(); ++x)
            {
                if (_clients[x].getNickname() == vec [i + 1]){
                     std::string nickError = ":servidor 433 " + _clients[x].getNickname() + " " + vec[i + 1] + " :Nickname is already in use";
                     MsgforHex(client.getSocketClient(), nickError);
                     return;
                }
            }
            std::string oldNick = client.getNickname();
            client.setNickname(vec[i + 1]);
            if(oldNick.empty())
                continue ;
            //std::cout << "Nickname changed from " << oldNick << " to " << client.getNickname() << std::endl;
            std::string response = ":" + oldNick + " NICK " + client.getNickname();
            MsgforHex(clientSocket, response);
        }
        else if (vec[i] == "JOIN")
        {
            std::string channelName = vec[i + 1];
            //printChannels();
            handleJoinCommand(client, channelName);
            infoForChannel(client, channelName);
        }
        else if(vec[i] == "PRIVMSG")
        {
            std::string channelName = vec[i + 1];
            std::string msg;
            for (size_t j = i + 2; j < vec.size(); j++)
                msg += vec[j];
            std:: cout << msg << std::endl;
        }
        else if(vec[i] == "PART"){

         }        
         else if(vec[i] == "QUIT"){

         }        
         else if(vec[i] == "WHO"){

         }        
        else if(vec[i] == "LIST"){

        }
        //////////////////// aqui  pra baixo paulo 
         else if(vec[i] == "KICK"){

         }        
        else if(vec[i] == "INVITE"){

        }
         else if(vec[i] == "TOPIC"){

        }        
         else if(vec[i] == "MODE"){

        }        


    }
}

void Server::runServer()
{
    while (1) {
        int result = poll(_fds.data(), _fds.size(), -1); // Espera indefinidamente por eventos
        if (result < 0) {
            std::cerr << "Error in poll()\n";
            break;
        }
        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].revents & POLLIN) {
                if (_fds[i].fd == _serverSocket) 
                    newClientConnection();
                else {
                    // Cliente existente enviando dados
                    std::string buff = readData(i);
                    if(buff.empty())
                        continue ;
                    std::cout << "Received: " << buff << std::endl;

                    


                    IrcMessages message(buff);
                    for (size_t j = 0; j < _clients.size(); ++j) {
                        if (_fds[i].fd == _clients[j].getSocketClient()) 
                        {
                            findCmd(message._vecMsg, _clients[j], _clients[j].getSocketClient());
                            //std::cerr << "Cliente é " << _clients[j].getNickname() << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void Server::createServerSocket()
{
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
}

void Server::inicializeServer() {
    
    createServerSocket();
    std::cout << "Server waiting for connections...\n";
    pollfd server;
    server.fd = _serverSocket;
    server.events = POLLIN; 
    _fds.push_back(server); //add the server socket to the set of descriptors to be monitored by poll()
    runServer();
    close(_serverSocket);
}


