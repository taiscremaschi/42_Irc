
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

std::vector<std::string> Channel::getAllClientsName()
{
    std::vector<std::string> result;    
    for(size_t i = 0; i < _clientsChannel.size(); ++i)
    {
        std::cout << "esse eh meu cliente" << _clientsChannel[i].getNickname() << std::endl;
        result.push_back(_clientsChannel[i].getNickname());
    }
    return result;
}

std::vector<Client> Channel::getAllClients()
{
    return _clientsChannel;
}