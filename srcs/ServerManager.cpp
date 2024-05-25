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

Channel *ServerManager::getChannelByName(const std::string &channel)
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

void ServerManager::infoForChannel(Client &client, std::string channelName) //funcao para mandar msg pra todos os clientes do channel avisando que um entrou 
{
    Channel *channel = getChannelByName(channelName);
    if(channel == NULL)
        return;
    std::vector<Client>  clients = channel->getAllClients();
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i].getNickname() != client.getNickname())
            MsgforHex(clients[i].getSocketClient(), MsgFormatIrc::joinMessage(client, channelName));
    }
}

void ServerManager::handleJoinCommand(Client& client, const std::string& channelName) //funcao para join
{
    Channel *channel = getChannelByName(channelName);
    Channel newchannel(channelName, client);
    if(channel == NULL)
    {
        _channels.push_back(newchannel);
        channel = &newchannel;
    }
    else
        channel->addClient(client);
    std::string namesList = ":server 353 " + client.getNickname() + " = " + channelName + " :";
    std::vector<std::string> vecClients = channel->getAllClientsName();
    for(size_t j = 0; j < vecClients.size(); ++j)
    {
        if(channel->searchOperator(vecClients[j]))
            namesList += "@";
        namesList += vecClients[j] + " ";
    }

    MsgforHex(client.getSocketClient(), MsgFormatIrc::joinMessage(client, channelName));
    MsgforHex(client.getSocketClient(), MsgFormatIrc::topicMessage(client, channelName, "Topic inicial of channel"));
    MsgforHex(client.getSocketClient(), MsgFormatIrc::topicCreatorMessage(client, channelName));
    MsgforHex(client.getSocketClient(), namesList);
    MsgforHex(client.getSocketClient(), MsgFormatIrc::endOfNameMessage(client, channelName));
}

bool ServerManager::changeNick(Client &client, const std::string &nick)
{
    //TODO: MANDAR MSG PRA TODOS OS CANAIS MUDANDO O NICK
    for(size_t x = 0; x < _clients.size(); ++x)
    {
        if (_clients[x].getNickname() == nick){
            MsgforHex(client.getSocketClient(), MsgFormatIrc::nickErrorMessage(_clients[x], nick));
            return false;
        }
    }
    std::string oldNick = client.getNickname();
    client.setNickname(nick);
    if(oldNick.empty())
        return false;
    MsgforHex(client.getSocketClient(), MsgFormatIrc::nickMessage(client, oldNick));
    return true;
}

void ServerManager::handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages)
{
    std::string msg = handleMsg(messages._message);
    if ( type[0] == '#')
    {
        Channel *ch = getChannelByName(type);
        if(ch == NULL){
            MsgforHex(client.getSocketClient(), MsgFormatIrc::privErrorMessage(client, type));
            return;
        }
        for(size_t i = 0; i < ch->getAllClients().size(); i++){
            if (ch->getAllClients()[i].getSocketClient() != client.getSocketClient())
                MsgforHex(ch->getAllClients()[i].getSocketClient(), MsgFormatIrc::privMessage(client, ch->getName(), msg));
        }
    }
    else {
        Client *receiver = getClientByNick(type);
        if(receiver == NULL){
            MsgforHex(client.getSocketClient(), MsgFormatIrc::privErrorMessage(client, type));
            return;
        }
        MsgforHex(receiver->getSocketClient(), MsgFormatIrc::privMessage(client, receiver->getNickname(), msg));
    }

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
            Channel *channel = getChannelByName(vec[i + 1]);
            if(channel == NULL){
                MsgforHex(client.getSocketClient(), MsgFormatIrc::partErrorMessage(client, vec[i + 1]));
                return;
            }
            for(size_t i = 0; i < channel->getAllClients().size(); i++){
                    MsgforHex(channel->getAllClients()[i].getSocketClient(), MsgFormatIrc::partMessage(client, channel, exitMsg));
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

