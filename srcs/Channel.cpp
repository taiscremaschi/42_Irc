#include "../includes/Channel.hpp"

Channel::Channel(const std::string &name, Client *client){
	_name = name;
	_operators.push_back(client);
	_topic = "Default topic";
	_inviteOnly = false;
	_topicOpOnly = true;
	_userLimit = 0;
	_limitEnabled = false;
	_clientsChannel.push_back(client);
	_keySet = false;
	_key = "";
	_newChannel = true;

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
	
bool Channel::addClient(Client *client)
{
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

bool Channel::isInviteOnly() const
{
	return (_inviteOnly);
}

void Channel::setInviteOnly(bool inviteOnly)
{
	_inviteOnly = inviteOnly;
}

bool Channel::isInvited(Client *client) const
{
	return (std::find(_invitedClients.begin(), _invitedClients.end(), client) != _invitedClients.end());
}

void Channel::addInvite(Client *client)
{
	if (!isInvited(client))
		_invitedClients.push_back(client);
}

void Channel::removeInvite(Client *client)
{
	for (size_t i = 0; i < _invitedClients.size(); ++i)
	{
		if (_invitedClients[i] == client)
		{
			_invitedClients.erase(_invitedClients.begin() + i);
			break;
		}
	}
}

bool Channel::searchOperator(const std::string &name){
	for(size_t i = 0; i < _operators.size(); ++i)
	{
		if(_operators[i]->getNickname() == name)
			return true;
	}
	return false;
}

void Channel::addOperator(Client *client)
{
	if (!searchOperator(client->getNickname()))
		_operators.push_back(client);
}

void Channel::removeOperator(Client *client)
{
	for (size_t i = 0; i < _operators.size(); ++i)
	{
		if (_operators[i] == client)
		{
			_operators.erase(_operators.begin() + i);
			break;
		}
	}
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

void Channel::setTopicOpOnly(bool value)
{
	_topicOpOnly = value;
}

bool Channel::isTopicOpOnly() const
{
	return (_topicOpOnly);
}

void Channel::setKey(const std::string &key)
{
	_key = key;
	_keySet = true;
}

void Channel::unsetKey(void)
{
	_key.clear();
	_keySet = false;
}

bool Channel::hasKey(void) const
{
	return (_keySet);
}

bool Channel::checkKey(const std::string &key) const
{
	return (_key == key);
}

void Channel::setUserLimit(int limit)
{
	if (limit > 0)
	{
		_userLimit = limit;
		_limitEnabled = true;
	}
	else
		_limitEnabled = false;
}

int Channel::getUserLimit(void) const
{
	return (_userLimit);
}

bool Channel::isLimitEnabled(void) const
{
	return (_limitEnabled);
}

int Channel::getClientNumber(void) const
{
	return (_clientsChannel.size());
}

bool Channel::isNew(void) const
{
	return (_newChannel);
}

std::string Channel::getModes() const {
	std::string result = "+";
	if(_inviteOnly){
		result += "i";
	}
	if(_topicOpOnly){
		result += "t";
	}
	if(_keySet)
		result += "k";
	if(_limitEnabled)
		result += "l";
	return result;
}