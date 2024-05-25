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

    MsgFormat::MsgforHex(client.getSocket(), MsgFormat::join(client, channelName));
    MsgFormat::MsgforHex(client.getSocket(), MsgFormat::topic(client, channelName, "Topic initial of channel"));
    MsgFormat::MsgforHex(client.getSocket(), MsgFormat::topicCreator(client, channelName));
    MsgFormat::MsgforHex(client.getSocket(), namesList);
    MsgFormat::MsgforHex(client.getSocket(), MsgFormat::endOfName(client, channelName));

    std::vector<Client>  clients = channel->getAllClients();
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i].getNickname() != client.getNickname())
            MsgFormat::MsgforHex(clients[i].getSocket(), MsgFormat::join(client, channelName));
    }
}

bool ServerManager::changeNick(Client &client, const std::string &nick)
{
    //TODO: MANDAR MSG PRA TODOS OS CANAIS MUDANDO O NICK
    for(size_t x = 0; x < _clients.size(); ++x)
    {
        if (_clients[x].getNickname() == nick){
            MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickError(_clients[x], nick));
            return false;
        }
    }
    std::string oldNick = client.getNickname();
    client.setNickname(nick);
    if(oldNick.empty())
        return false;
    MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nick(client, oldNick));
    return true;
}

//TODO: preciso ver a questao de quando eu tiver no nome do nick ou channel : preciso ter certexza de onde começa my msg
///TODO: o programa esta mandando msg pra grupo que n faz parte precisa de protecao. 
void ServerManager::handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages)
{
    if ( type[0] == '#')
    {
        Channel *ch = getChannelByName(type);
        if(ch == NULL){
            MsgFormat::MsgforHex(client.getSocket(), MsgFormat::privError(client, type));
            return;
        }
        std::vector<Client> clients = ch->getAllClients();
        for(size_t i = 0; i < clients.size(); i++){
            if (clients[i].getSocket() != client.getSocket())
                MsgFormat::MsgforHex(clients[i].getSocket(), MsgFormat::priv(client, ch->getName(), MsgFormat::handleMsg(messages._message)));
        }
    }
    else {
        Client *receiver = getClientByNick(type);
        if(receiver == NULL){
            MsgFormat::MsgforHex(client.getSocket(), MsgFormat::privError(client, type));
            return;
        }
        MsgFormat::MsgforHex(receiver->getSocket(), MsgFormat::priv(client, receiver->getNickname(), MsgFormat::handleMsg(messages._message)));
    }
}

void ServerManager::handlePart(Client& client, IrcMessages &messages,const std::string& channelName)
{
    Channel *channel = getChannelByName(channelName);
    if(channel == NULL){
        MsgFormat::MsgforHex(client.getSocket(), MsgFormat::partError(client, channelName));
        return;
    }
    std::vector<Client> clients = channel->getAllClients();
    for(size_t i = 0; i < clients.size(); i++){
        MsgFormat::MsgforHex(clients[i].getSocket(), MsgFormat::part(client, channel, MsgFormat::handleMsg(messages._message)));
    }
    channel->removeClient(client);
}

void ServerManager::findCmd(const std::vector<std::string> &vec, Client &client, IrcMessages &messages) {
    
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == "NICK") 
        {
            changeNick(client, vec[++i]);
            return;
        }
        else if (vec[i] == "JOIN"){
            handleJoinCommand(client, vec[i + 1]);
            return;
        }
        else if(vec[i] == "PRIVMSG"){
            handlePrivMessage(client, vec[i + 1], messages);
            return;
        }
        else if(vec[i] == "PART"){
            handlePart(client, messages, vec[i + 1]);
            return;
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
        if (fd == _clients[j].getSocket()) 
            findCmd(message._vecMsg, _clients[j], message);
    }
}

