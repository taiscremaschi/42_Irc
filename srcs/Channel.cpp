
#include "Channel.hpp"

Channel::Channel(const std::string &name, const Client &client){
    _name = name;
    _operators.push_back(client);
    _topic = name;
    
}

Channel::~Channel(){}

const std::string &Channel::getName() const {
    return _name;

}
    
void Channel::addClient(Client &client){

    _clientsChannel.push_back(client);
}
