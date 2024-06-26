#include "ServerManager.hpp"
#include "Server.hpp"

ServerManager::ServerManager(){}

ServerManager::~ServerManager(){
    for(size_t i=0; i< _channels.size(); ++i){
        delete _channels[i];
    }
    for(size_t i=0; i< _clients.size(); ++i){
        delete _clients[i];
    }
}

void ServerManager::createClient(Client *client){
    _clients.push_back(client);
}

void ServerManager::removeClientByNick(std::string nick){

    for(size_t i = 0; i < _clients.size(); ++i){
        if(_clients[i]->getNickname() == nick){
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
}

void ServerManager::removeClientByFd(int fd){

    for(size_t i = 0; i < _clients.size(); ++i){
        if(_clients[i]->getSocket() == fd){
            close(_clients[i]->getSocket());
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
}

void ServerManager::removeClient(int i){
     delete _clients[i];
     _clients.erase(_clients.begin() + i);
}

Client *ServerManager::getClientByNick(const std::string &nick)
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i]->getNickname() == nick)
            return _clients[i];
    }
    return NULL;
}

Channel *ServerManager::getChannelByName(const std::string &channel)
{
    for (size_t i = 0; i < _channels.size(); ++i)
    {
        if (_channels[i]->getName() == channel)
            return _channels[i];
    }
    return NULL;
}

void ServerManager::handleJoinCommand(Client& client, const std::string& channelName)
{
    Channel *channel = getChannelByName(channelName);
    if(channel == NULL)
    {
        Channel *newchannel = new Channel(channelName, &client);
        _channels.push_back(newchannel);
        channel = newchannel;
    }
    else
    {
        if(!channel->addClient(&client))
            return;
    }
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

    std::vector<Client*>  clients = channel->getAllClients();
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i]->getNickname() != client.getNickname())
            MsgFormat::MsgforHex(clients[i]->getSocket(), MsgFormat::join(client, channelName));
    }
}


bool ServerManager::changeNick(Client &client, const std::string &nick)
{

    for(size_t x = 0; x < _clients.size(); ++x)
    {
        if (_clients[x]->getNickname() == nick){
            MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickError(*_clients[x], nick));
            return false;
        }
    }
    std::string oldNick = client.getNickname();
    client.setNickname(nick);
    MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nick(client, oldNick));
    for(size_t i = 0; i < _channels.size(); ++i)
    {
        if(_channels[i]->searchNames(client.getNickname()) == true)
            _channels[i]->sendMessageToClients(MsgFormat::notifyNickChanged(client, oldNick));
    }
    return true;
}

void ServerManager::handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages)
{
    if ( type[0] == '#' || type[0] == '&')
    {
        Channel *ch = getChannelByName(type);
        if(ch == NULL)
        {
            MsgFormat::MsgforHex(client.getSocket(), MsgFormat::privError(client, type));
            return;
        }
        if (ch->searchNames(client.getNickname()))
        {
            std::vector<Client*> clients = ch->getAllClients();
            for(size_t i = 0; i < clients.size(); i++)
            {
                if (clients[i]->getSocket() != client.getSocket()) 
                    MsgFormat::MsgforHex(clients[i]->getSocket(), MsgFormat::priv(client, ch->getName(), MsgFormat::handleMsg(messages._message)));

            }
        }
        else
            MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notifyUserNotInChannel(client, ch->getName()));
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
    channel->sendMessageToClients(MsgFormat::part(client, channel, MsgFormat::handleMsg(messages._message)));
    channel->removeClient(&client);
}

void ServerManager::handleQuit(Client& client, IrcMessages &quitMsg)
{
    for(size_t i = 0; i < _channels.size(); i++)
    {
        if(_channels[i]->searchNames(client.getNickname())){
            _channels[i]->removeClient(&client);
            _channels[i]->sendMessageToClients(MsgFormat::quit(client, MsgFormat::handleMsg(quitMsg._message)));
        }
    }
    close(client.getSocket());
    removeClientByNick(client.getNickname());
}

bool ServerManager::handlePass(Client& client, std::string pass, std::string vec)
{
    if (vec != pass){
        MsgFormat::MsgforHex(client.getSocket(), MsgFormat::passInvalid());
        removeClientByFd(client.getSocket());
        return false;
    }
    else {
        client.isAuthenticated();
        MsgFormat::MsgforHex(client.getSocket(), MsgFormat::passValid());
        return true;
    }
}

void ServerManager::findCmd(const std::vector<std::string> &vec, Client &client, IrcMessages &messages, std::string pass) {
    
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == "PASS" && (vec.size() > i + 1))
        {
            if (!handlePass(client, pass, vec[i + 1]))
                return;
        }
        else if ((vec[i] == "NICK" && (vec.size() > i + 1)) && client.getAuthenticated()) 
            changeNick(client, vec[++i]);
        else if ((vec[i] == "USER" && (vec.size() > i + 1) ) && client.getAuthenticated()) 
            client.setName(vec[i + 1]);
        else if ((vec[i] == "JOIN" && (vec.size() > i + 1)) && client.checkLoginData())
        {
            if(vec[i + 1][0] != '#' && vec[i + 1][0] != '&')
                continue;
            handleJoinCommand(client, vec[++i]);
        }
        else if((vec[i] == "PRIVMSG" && (vec.size() > i + 1)) && client.checkLoginData())
            handlePrivMessage(client, vec[++i], messages);
        else if((vec[i] == "PART" && (vec.size() > i + 1)) && client.checkLoginData())
            handlePart(client, messages, vec[++i]);
        else if(vec[i] == "QUIT")
            handleQuit(client, messages);
        //////////////////// aqui  pra baixo paulo  ///
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

void ServerManager::handleIrcCmds(std::string buff, int fd, std::string pass){
    for (size_t j = 0; j < _clients.size(); ++j) {
        if (fd == _clients[j]->getSocket()) {
            if(_clients[j]->saveBuffer(buff))
            {
                IrcMessages message(_clients[j]->getBuffer());
                findCmd(message._vecMsg, *_clients[j], message, pass);
                _clients[j]->clearBuffer();
                
            }
        }
    }
}

