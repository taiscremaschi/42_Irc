
#include "Channel.hpp"

Channel::Channel(const std::string &name, const Client &client){
    _name = name;
    _operators.push_back(client);
    _topic = name;
    _clientsChannel.push_back(client);
}

Channel::~Channel(){}

const std::string &Channel::getName() const {
    return _name;

}
    
void Channel::addClient(Client &client){
    _clientsChannel.push_back(client);
}

void Channel::removeClient(Client &client){
    for(size_t i = 0; i < _clientsChannel.size(); ++i)
    {
        if (_clientsChannel[i] == client)
            _clientsChannel.erase(_clientsChannel.begin() + i);
    }
}

std::vector<Client>  Channel::getOperator() const{
    return _operators;
}

std::vector<Client> Channel::getAllClients(){
    return _clientsChannel;
}

std::vector<std::string> Channel::getAllClientsName()
{
    std::vector<std::string> result;    
    for(size_t i = 0; i < _clientsChannel.size(); ++i)
        result.push_back(_clientsChannel[i].getNickname());
    return result;
}

bool Channel::searchOperator(const std::string &name){
    for(size_t i = 0; i < _operators.size(); ++i)
    {
        if(_operators[i].getNickname() == name)
            return true;
    }
    return false;
}

