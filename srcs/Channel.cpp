#include "../includes/Channel.hpp"

Channel::Channel(const std::string &name, Client *client){
	_name = name;
	_operators.push_back(client);
	_topic = "Default topic";
	_clientsChannel.push_back(client);
}

Channel::~Channel(){}

const std::string &Channel::getName() const {
	return _name;
}

const std::string &Channel::getTopic() const
{	
	return (this->_topic);
}

void Channel::setTopic(const std::string &topic)
{
	this->_topic = topic;
}
	
bool Channel::addClient(Client *client){

	for(size_t i = 0; i < _clientsChannel.size(); ++i)
	{
		if (_clientsChannel[i] == client)
		   return false;
	}
	_clientsChannel.push_back(client);
	return true;
}

void Channel::removeClient(Client *client){
	for(size_t i = 0; i < _clientsChannel.size(); ++i)
	{
		if (_clientsChannel[i] == client)
			_clientsChannel.erase(_clientsChannel.begin() + i);
	}
}

std::vector<Client*>  &Channel::getOperator() {
	return _operators;
}

std::vector<Client*> Channel::getAllClients(){
	return _clientsChannel;
}

std::vector<std::string> Channel::getAllClientsName()
{
	std::vector<std::string> result;	
	for(size_t i = 0; i < _clientsChannel.size(); ++i)
		result.push_back(_clientsChannel[i]->getNickname());
	return result;
}

bool Channel::searchOperator(const std::string &name){
	for(size_t i = 0; i < _operators.size(); ++i)
	{
		if(_operators[i]->getNickname() == name)
			return true;
	}
	return false;
}

bool Channel::searchNames(const std::string name){
	for(size_t i = 0; i <  _clientsChannel.size(); ++i)
	{
		if( _clientsChannel[i]->getNickname() == name)
			return true;
	}
	return false;
}

void Channel::sendMessageToClients(std::string msg)
{
	 for(size_t j = 0; j < _clientsChannel.size(); j++)
	 {
		MsgFormat::MsgforHex(_clientsChannel[j]->getSocket(), msg);
	 }
}