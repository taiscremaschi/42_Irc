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


std::string ServerManager::channelExists(Client& client, const std::string& channelName){
    for(size_t i = 0; i < _channels.size(); ++i)
    {
        if(_channels[i].getName() == channelName)
        {
            //TODO: ADC O @ APENAS AO OPERADOR
            std::string namesList = ":server 353 " + client.getNickname() + " = " + channelName + " :";
            _channels[i].addClient(client);
            std::vector<std::string> result = _channels[i].getAllClientsName();
            for(size_t j = 0; j < result.size(); ++j)
                namesList += result[j] + " ";
            return namesList;
        }
    }
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
    size_t i =  0;
    for(; i < _channels.size(); ++i)
    {
        if(_channels[i].getName() == channelName)
            break;
    }
    if (i == _channels.size())
        return ;
    std::vector<Client>  clients = _channels[i].getAllClients();
    std:: string msg = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName; 
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i].getNickname() != client.getNickname())
            MsgforHex(clients[i].getSocketClient(), msg);
    }
}

void ServerManager::handleJoinCommand(Client& client, const std::string& channelName) 
{
    std::string namesMessage  = channelExists(client, channelName);
    if(namesMessage.empty()){
        Channel newchannel(channelName, client);
        _channels.push_back(newchannel);
        namesMessage = ":server 353 " + client.getNickname() + " = " + channelName + " :@" + client.getNickname();
    }

    std::string joinMsg = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName;
    std::string topicMsg = ":server 332 " + client.getNickname() + " " + channelName + " :Topic inicial of channel";
    std::string topicCreatorMsg = ":server 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " 0";
    std::string endOfNameMsg = ":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.";

    MsgforHex(client.getSocketClient(), joinMsg);
    MsgforHex(client.getSocketClient(), topicMsg);
    MsgforHex(client.getSocketClient(), topicCreatorMsg);
    MsgforHex(client.getSocketClient(), namesMessage);
    MsgforHex(client.getSocketClient(), endOfNameMsg);
}

bool ServerManager::changeNick(Client &client, const std::string &nick)
{
    for(size_t x = 0; x < _clients.size(); ++x)
    {
        if (_clients[x].getNickname() == nick){
                std::string nickError = ":server 433 " + _clients[x].getNickname() + " " + nick + " :Nickname is already in use";
                MsgforHex(client.getSocketClient(), nickError);
                return false;
        }
    }
    std::string oldNick = client.getNickname();
    client.setNickname(nick);
    if(oldNick.empty())
        return false;
    std::string response = ":" + oldNick + " NICK " + client.getNickname();
    MsgforHex(client.getSocketClient(), response);
    return true;
}

std::string handleMsg(std::string msg)
{
    int i = 0;
    while(msg[i] != ':')
        i++;
    i++;
    // protecao aqui
    std::string result = msg.substr(i, msg.length() - i);
    std::cout   << "sera que esse resul esta certo? " << result << std::endl;
    return result;
}

Client *ServerManager::handleNickricardo(const std::vector<std::string> &vec, int i)
{
    int j = 0;
    while(vec[i] != _clients[j].getNickname())
    {
        j++;
    }
    if(vec[i] == _clients[j].getNickname())
        return &_clients[j];
    return NULL;
}

void ServerManager::findCmd(const std::vector<std::string> &vec, Client &client, IrcMessages &messages) {
    
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == "NICK") 
        {
            changeNick(client, vec[++i]);
            return;
        }
        else if (vec[i] == "JOIN")
        {
            std::string channelName = vec[i + 1];
            handleJoinCommand(client, channelName);
            infoForChannel(client, channelName);
        }
        else if(vec[i] == "PRIVMSG")
        {
            std::string type = vec[i + 1];
            if ( type[0] == '#')
            {
                //funca para mensagem em grupo
            }
            else 
            {
                //funcao pra mensagem privada 
                std::string result = handleMsg(messages._message);
                Client *nickricardo = handleNickricardo(vec,(i+ 1));
                if(nickricardo == NULL)
                {
                    return;
                }
                //std:: cout << result << std::endl;
                std::string temp = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PRIVMSG " + nickricardo->getNickname() + ": " + result;
                MsgforHex(nickricardo->getSocketClient(), temp);
            }
            // std::string msg;
            // for (size_t j = i + 2; j < vec.size(); j++)
            //     msg += vec[j];



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

void ServerManager::handleIrcCmds(std::string buff, int fd){
    
    IrcMessages message(buff);
    for (size_t j = 0; j < _clients.size(); ++j) {
        if (fd == _clients[j].getSocketClient()) 
            findCmd(message._vecMsg, _clients[j], message);
    }

}