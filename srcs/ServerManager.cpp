#include "ServerManager.hpp"

ServerManager::ServerManager(){}

ServerManager::~ServerManager(){}

// void        ServerManager::printChannels()
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

void ServerManager::createClient(Client &client){
    _clients.push_back(client);
}

void ServerManager::removeClient(int i){
    _clients.erase(_clients.begin() + i);
}


void ServerManager::handleIrcCmds(std::string buff, int fd){
    IrcMessages message(buff);
    for (size_t j = 0; j < _clients.size(); ++j) {
    if (fd == _clients[j].getSocketClient()) 
    {
        findCmd(message._vecMsg, _clients[j], _clients[j].getSocketClient());
        //std::cerr << "Cliente é " << _clients[j].getNickname() << std::endl;
    }
}

}

std::string ServerManager::channelExists(Client& client, const std::string& channelName){
    for(size_t i = 0; i < _channels.size(); ++i)
    {
        if(_channels[i].getName() == channelName)
        {
            //TODO: ADC O @ APENAS AO OPERADOR
            std::string namesList = ":server 353 " + client.getNickname() + " = " + channelName + " :";
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

void MsgforHex(int clientSocket, const std::string& message) 
{
    std::string msg = message + "\r\n";
    std::cout << msg << std::endl;
    send(clientSocket, msg.c_str(), msg.length(), 0); //funcao para mandar mensagem pra outro socket
}


void ServerManager::infoForChannel(Client &client, std::string channelName)
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

void ServerManager::handleJoinCommand(Client& client, const std::string& channelName) 
{

    // std::cout << "antes do chanel exist meu vec + 1 is   " << channelName  << std::endl;
    std::string namesMessage  = channelExists(client, channelName);
    if(namesMessage.empty()){
        Channel newchannel(channelName, client);
        _channels.push_back(newchannel);
        //std::cout << "meu novo chanel " << newchannel.getName() << std::endl;
        namesMessage = ":server 353 " + client.getNickname() + " = " + channelName + " :@" + client.getNickname();
    }
    std::string joinMsg = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName;
    std::string topicMsg = ":server 332 " + client.getNickname() + " " + channelName + " :Topic inicial of channel";

    std::string topicCreatorMsg = ":server 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " 0";
    std::string endOfNameMsg = ":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.";


    //printChannels();

    MsgforHex(client.getSocketClient(), joinMsg);
    MsgforHex(client.getSocketClient(), topicMsg);
    MsgforHex(client.getSocketClient(), topicCreatorMsg);
    MsgforHex(client.getSocketClient(), namesMessage);
    MsgforHex(client.getSocketClient(), endOfNameMsg);

}

void ServerManager::findCmd(const std::vector<std::string> &vec, Client &client, int clientSocket) {
    
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
                     std::string nickError = ":server 433 " + _clients[x].getNickname() + " " + vec[i + 1] + " :Nickname is already in use";
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
