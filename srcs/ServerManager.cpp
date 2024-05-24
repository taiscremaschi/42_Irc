#include "ServerManager.hpp"

ServerManager::ServerManager(){}

ServerManager::~ServerManager(){}

void ServerManager::createClient(Client &client){
    _clients.push_back(client);
}

void ServerManager::removeClient(int i){
    _clients.erase(_clients.begin() + i);
}

Client *ServerManager::getClientByNick(const std::string &nick)
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i].getNickname() == nick)
            return &_clients[i];
    }
    return NULL;
}

Channel *ServerManager::getChannelByNick(const std::string &channel)
{
    for (size_t i = 0; i < _channels.size(); ++i)
    {
        if (_channels[i].getName() == channel)
            return &_channels[i];
    }
    return NULL;
}

std::string handleMsg(std::string msg)
{
    int i = 0;
    while(msg[i] && msg[i] != ':')
       i++;
    if(msg[i])
        i++;
    std::string result = msg.substr(i, msg.length() - i);
    return result;
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
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i].getNickname() != client.getNickname())
            MsgforHex(clients[i].getSocketClient(), MsgFormatIrc::joinMessage(client, channelName));
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

    MsgforHex(client.getSocketClient(), MsgFormatIrc::joinMessage(client, channelName));
    MsgforHex(client.getSocketClient(), MsgFormatIrc::topicMessage(client, channelName, "Topic inicial of channel"));
    MsgforHex(client.getSocketClient(), MsgFormatIrc::topicCreatorMessage(client, channelName));
    MsgforHex(client.getSocketClient(), namesMessage);
    MsgforHex(client.getSocketClient(), MsgFormatIrc::endOfNameMessage(client, channelName));
}

bool ServerManager::changeNick(Client &client, const std::string &nick)
{
    //TODO: MANDAR MSG PRA TODOS OS CANAIS MUDANDO O NICK
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

void ServerManager::handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages)
{
    if ( type[0] == '#')
    {
        std::string msgGruop = handleMsg(messages._message);
        Channel *ch = getChannelByNick(type);
        if(ch == NULL){
            std::string erroMsg =  ":server 401 " + client.getNickname() + " " + type + " :No such nick/channel";
            MsgforHex(client.getSocketClient(), erroMsg);
            return;
        }
        std::string myMsg = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PRIVMSG " + ch->getName() + " :" + msgGruop;
        for(size_t i = 0; i < ch->getAllClients().size(); i++){
            if (ch->getAllClients()[i].getSocketClient() != client.getSocketClient())
                MsgforHex(ch->getAllClients()[i].getSocketClient(), myMsg);
        }
    }
    else {
        std::string result = handleMsg(messages._message);
        Client *receiver = getClientByNick(type);
        if(receiver == NULL){
            std::string erroMsg =  ":server 401 " + client.getNickname() + " " + type + " :No such nick/channel";
            MsgforHex(client.getSocketClient(), erroMsg);
            return;
        }
        std::string hexMessage = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PRIVMSG " + receiver->getNickname() + ": " + result;
        MsgforHex(receiver->getSocketClient(), hexMessage);
    }

}

//TODO: mudat o nome da funcao pra ficar mais claro o que ela faz (PODE IR PARA O CHANNEL)
std::string ServerManager::channelExists(Client& client, const std::string& channelName)
{
    for(size_t i = 0; i < _channels.size(); ++i)
    {
        if(_channels[i].getName() == channelName)
        {
            std::string namesList = ":server 353 " + client.getNickname() + " = " + channelName + " :";
            _channels[i].addClient(client);
            std::vector<std::string> vecClients = _channels[i].getAllClientsName();
            for(size_t j = 0; j < vecClients.size(); ++j)
            {
                if(_channels[i].searchOperator(vecClients[j]))
                    namesList += "@";
                namesList += vecClients[j] + " ";
            }
            return namesList;
        }
    }
    return "";
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
            //TODO: preciso ver a questao de quando eu tiver no nome do nick ou channel : preciso ter certexza de onde começa my msg
            ///TODO: o programa esta mandando msg pra grupo que n faz parte precisa de protecao. 
            std::string type = vec[i + 1];
            handlePrivMessage(client, type, messages);
            return;

        }
        else if(vec[i] == "PART"){
            std::string exitMsg = handleMsg(messages._message);
            Channel *channel = getChannelByNick(vec[i + 1]);
            if(channel == NULL){
                std::string error = ":server 403 " + client.getNickname() + " " + vec[i + 1] + " :No such channel";
                MsgforHex(client.getSocketClient(), error);
                return;
            }
            std::string msgPart = ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PART " + channel->getName() + " :" + exitMsg;
            for(size_t i = 0; i < channel->getAllClients().size(); i++){
                    MsgforHex(channel->getAllClients()[i].getSocketClient(), msgPart);
                }
            channel->removeClient(client);
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

