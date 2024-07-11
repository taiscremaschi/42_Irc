#include "../includes/ServerManager.hpp"
#include "../includes/Server.hpp"
#include <string>

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

void ServerManager::handleJoinCommand(Client& client, const std::string& channelName, const std::string &key)
{
	Channel *channel = getChannelByName(channelName);
	if (!channel)
	{
		Channel *newchannel = new Channel(channelName, &client);
		_channels.push_back(newchannel);
		channel = newchannel;
	}
	else if (channel->isLimitEnabled() && (channel->getClientNumber() >= channel->getUserLimit()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelFull(client, channelName));
		return;
	}	
	else
		channel->addClient(&client);

	if (channel->isInviteOnly() && !channel->isInvited(&client))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::inviteOnlyChannel(client, channelName));
		channel->removeClient(&client);
		return;
	}
	if (channel->hasKey() && !channel->checkKey(key))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidKey(client, channelName));
		channel->removeClient(&client);
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
	MsgFormat::MsgforHex(client.getSocket(), MsgFormat::topic(client, channelName, channel->getTopic()));
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
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userNotFound(client, type));
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
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userNotFound(client, type));
			return;
		}
		MsgFormat::MsgforHex(receiver->getSocket(), MsgFormat::priv(client, receiver->getNickname(), MsgFormat::handleMsg(messages._message)));
	}
}

void ServerManager::handlePart(Client& client, IrcMessages &messages,const std::string& channelName)
{
	Channel *channel = getChannelByName(channelName);
	if(channel == NULL){
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
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


void ServerManager::handleTopic(Client &client, const std::vector<std::string> &vec, size_t i)
{
	std::string channelName = vec[i];
	std::string topic = "";
	Channel *channel = getChannelByName(channelName);
	if (!channel)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
		return;
	}
	else if (vec[++i].empty())
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::topic(client, channelName, channel->getTopic()));
		return;
	}

	if (vec.size() > i + 1)
	{
		for (size_t j = i; j < vec.size(); j++)
			topic += vec[j] + " ";

		if (!topic.empty() && topic[topic.size() - 1] == ' ')
			topic.erase(topic.size() - 1);
		if (!topic.empty() && topic[0] == ':')
			topic = topic.substr(1);
	}

	std::cout << "topic: " << topic << std::endl;

	if (!channel->searchNames(client.getNickname()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::youNotInChannel(client, channelName));
		return;
	}
	else if (channel->isTopicOpOnly() && !channel->searchOperator(client.getName()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notChannelOperator(client, channelName));
		return;
	}

	channel->setTopic(topic);
	channel->sendMessageToClients(MsgFormat::topic(client, channelName, topic));
	channel->sendMessageToClients(MsgFormat::topicCreator(client, channelName));
}

void ServerManager::handleKick(Client &client, const std::string &channelName, const std::string &targetNick, const std::vector<std::string> &vec, size_t i)
{
	Channel *channel = getChannelByName(channelName);
	if (!channel)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
		return;
	}	
	else if (!channel->searchOperator(client.getNickname()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notChannelOperator(client, channelName));
		return;
	}

	Client *target = getClientByNick(targetNick);
	if (!target)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickNotFound(client, targetNick));
		return;
	}
	else if (!channel->searchNames(targetNick))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userNotInChannel(client, channelName, targetNick));
	}

	if (channel->searchOperator(target->getNickname()))
		channel->removeOperator(target);
	if (channel->isInvited(target))
		channel->removeInvite(target);
	std::string reason;
	for (size_t j = i; j < vec.size(); ++j)
		reason += vec[j] + " ";
	if (reason[0] == ':')
		reason = reason.substr(1);
	if (!reason.empty() && reason[reason.size() - 2] == ' ')
		reason.erase(reason.size() - 2);

	std::string kickMsg = MsgFormat::kickUser(client, channelName, targetNick, reason);
	channel->sendMessageToClients(kickMsg);
	channel->removeClient(target);
}

void ServerManager::handleInvite(Client &client, const std::string &targetNick, const std::string &channelName)
{
	Channel *channel = getChannelByName(channelName);
	if (!channel)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
		return;
	}
	else if (!channel->searchNames(client.getNickname()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userNotInChannel(client, channelName, targetNick));
		return;
	}
	else if (!channel->searchOperator(client.getNickname()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notChannelOperator(client, channelName));
		return;
	}

	Client	*target = getClientByNick(targetNick);
	if (!target)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickNotFound(client, targetNick));
		return;
	}
	else if (channel->searchNames(targetNick))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userAlreadyInChannel(client, targetNick, channelName));
		return;
	}

	std::string inviteMsg = MsgFormat::invite(client, channelName, targetNick);
	MsgFormat::MsgforHex(target->getSocket(), inviteMsg);
	MsgFormat::MsgforHex(client.getSocket(), MsgFormat::inviteConfirm(client, channelName, targetNick));
	channel->addInvite(target);
}

void ServerManager::handleMode(Client &client, std::vector<std::string> vec, size_t i)
{
	std::string channelName = vec[i++];
	std::string mode = vec[i++];
	Channel	*channel = getChannelByName(channelName);
	if (!channel)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
		return;
	}
	else if (!channel->searchOperator(client.getNickname()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notChannelOperator(client, channelName));
		return;
	}

	if (mode.size() != 2 && mode[0] != '+' && mode[0] != '-')
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::unsupportedMode(client, channelName, mode));
		return;
	}

	bool set = (mode[0] == '+');
	char modeFlag = mode[1];
	if (modeFlag == 'i')
		channel->setInviteOnly(set);
	else if (modeFlag == 't')
		channel->setTopicOpOnly(set);
	else if (modeFlag == 'o')
	{
		std::string targetNick = vec[i];
		Client *target = getClientByNick(targetNick);
		if (!target)
		{
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userNotFound(client, targetNick));
			return;
		}

		set ? channel->addOperator(target) : channel->removeOperator(target);
		channel->sendMessageToClients(MsgFormat::changeOpStatus(client, target, channelName, set));
		return;
	}
	else if (modeFlag == 'l')
	{
		if (vec[i].empty())
		{
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidModeParams(client, channelName, mode));
			return;
		}

		for (size_t j = 0; vec[i][j]; j++)
		{
			if (vec[i].c_str()[j] < '0' || vec[i].c_str()[j] > '9')
			{
				MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidModeParams(client, channelName, mode));
				return;
			}
		}

		channel->setUserLimit(set ? std::atoi(vec[i].c_str()) : 0);
	}
	else if (modeFlag == 'k')
	{
		std::string key = vec[i];
		if (set && key.empty())
		{
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidModeParams(client, channelName, mode));
			return;
		}

		if (set)
		{
			channel->setKey(key);
			channel->sendMessageToClients(MsgFormat::keySet(client, channelName, key));
			return;
		}
		else
			channel->unsetKey();
	}
	else
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::unsupportedMode(client, channelName, mode));
		return;
	}

	std::string modeMsg = mode[0] + std::string(1, modeFlag);
	channel->sendMessageToClients(MsgFormat::mode(client, channelName, modeMsg));
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
			handleJoinCommand(client, vec[i + 1], vec[i + 2]);
			i += 2;
		}
		else if((vec[i] == "PRIVMSG" && (vec.size() > i + 1)) && client.checkLoginData())
			handlePrivMessage(client, vec[++i], messages);
		else if((vec[i] == "PART" && (vec.size() > i + 1)) && client.checkLoginData())
			handlePart(client, messages, vec[++i]);
		else if(vec[i] == "QUIT" && vec.size() > i + 2 && client.checkLoginData())
			handleQuit(client, messages);
		//////////////////// aqui  pra baixo paulo  ///
		else if(vec[i] == "KICK" && vec.size() > i + 2 && client.checkLoginData()){
			handleKick(client, vec[i + 1], vec[i + 2], vec, i + 3);
		}		
		else if(vec[i] == "INVITE" && vec.size() > i + 2 && client.checkLoginData()){
			handleInvite(client, vec[i + 1], vec[i + 2]);
			i += 2;
		}
		else if(vec[i] == "TOPIC" && (vec.size() > i + 1) && client.checkLoginData()){
			handleTopic(client, vec, ++i);
		}		
		else if(vec[i] == "MODE" && vec.size() > i + 2 && client.checkLoginData()){
			handleMode(client, vec, ++i);
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
