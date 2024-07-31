#include "../includes/ServerManager.hpp"
#include "../includes/Server.hpp"

ServerManager::ServerManager(){


}

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
	
	handleQuit(*_clients[i], ":leaving");
	// delete _clients[i];
	// _clients.erase(_clients.begin() + i);
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

Client *ServerManager::getClientByUser(const std::string &user)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getName() == user)
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

void ServerManager::handleJoinCommand(Client& client, const std::string& channelName, const std::vector<std::string> &vec)
{
	std::string key;
	(vec.size() == 2) ? key = "" : key = vec[2];

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
	else if (!channel->addClient(&client))
		return;

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


void ServerManager::changeNick(Client &client, const std::string &nick)
{
	for(size_t x = 0; x < _clients.size(); ++x)
	{
		if (_clients[x]->getNickname() == nick){
			if(client.getNickname().empty()){
				client.setNickname(nick);
				client.setNickError(true);
			}
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickError(client, nick));
			return ;
		}
	}
	std::string oldNick = client.getNickname();
	client.setNickname(nick);
	client.setNickError(false);
	MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nick(client, oldNick));
	for(size_t i = 0; i < _channels.size(); ++i)
	{
		if(_channels[i]->searchNames(client.getNickname()) == true)
			_channels[i]->sendMessageToClients(MsgFormat::notifyNickChanged(client, oldNick));
	}
}

void ServerManager::handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages)
{
	if ( type[0] == '#' || type[0] == '&')
	{
		Channel *ch = getChannelByName(type);
		if(ch == NULL)
		{
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickNotFound(client, type));
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
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickNotFound(client, type));
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
	if(!channel->searchNames(client.getNickname())){
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notifyUserNotInChannel(client, channel->getName()));
		return;
	}

	channel->sendMessageToClients(MsgFormat::part(client, channel, MsgFormat::handleMsg(messages._message)));
	channel->removeOperator(&client);
	channel->removeClient(&client);
}

bool ServerManager::handleQuit(Client& client, const std::string &quitMsg)
{
	if(client.getNickError() == true){
		removeClientByFd(client.getSocket());
		return false;
	}
	for(size_t i = 0; i < _channels.size(); i++)
	{
		if(_channels[i]->searchNames(client.getNickname())){
			_channels[i]->removeClient(&client);
			_channels[i]->removeOperator(&client);
			_channels[i]->sendMessageToClients(MsgFormat::part(client, _channels[i], MsgFormat::handleMsg(quitMsg)));
		}
	}
	close(client.getSocket());
	removeClientByNick(client.getNickname());
	return false;
}

void ServerManager::handlePass(Client& client, std::string pass, std::string vec)
{
	if (vec != pass){
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::passInvalid());
		return;
	}
	else {
		client.isAuthenticated();
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::passValid());
		return;
	}
}


void ServerManager::handleTopic(Client &client, const std::vector<std::string> &vec)
{
	std::string channelName = vec[1];
	std::string topic = "";
	Channel *channel = getChannelByName(channelName);
	if (!channel)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
		return;
	}
	else if (vec[2].empty())
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::topic(client, channelName, channel->getTopic()));
		return;
	}

	if (vec.size() >= 3)
	{
		for (size_t j = 2; j < vec.size(); j++)
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
	else if (channel->isTopicOpOnly() && !channel->searchOperator(client.getNickname()))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::notChannelOperator(client, channelName));
		return;
	}

	channel->setTopic(topic);
	channel->sendMessageToClients(MsgFormat::topic(client, channelName, topic));
	channel->sendMessageToClients(MsgFormat::topicCreator(client, channelName));
}

void ServerManager::handleKick(Client &client, const std::string &channelName, const std::string &targetNick, const std::vector<std::string> &vec)
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
	if (!channel->searchNames(targetNick))
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userNotInChannel(client, channelName, targetNick));
		return;
	}
	if (channel->isInvited(target))
		channel->removeInvite(target);
	if (channel->searchOperator(targetNick))
		channel->removeOperator(target);

	std::string reason;
	for (size_t j = 3; j < vec.size(); ++j)
		reason += vec[j] + " ";
	if (reason[0] == ':')
		reason = reason.substr(1);
	if (!reason.empty() && reason[reason.size() - 2] == ' ')
		reason.erase(reason.size() - 2);
	else
		reason = "Kicked by " + client.getName();

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

void ServerManager::handleMode(Client &client, std::vector<std::string> vec)
{
	std::string channelName = vec[1];
	std::string mode;
	if (vec.size() > 2 )
		mode = vec[2];
	if (mode.empty())
		return;
	Channel	*channel = getChannelByName(channelName);
	if (!channel)
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::channelNotFound(client, channelName));
		return;
	}
	/*
	else if (channel->isNew())
	{
		channel->setNew(false);
		return;
	}
	*/
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
		if (3 > vec.size() - 1)
			return;
		std::string targetNick = vec[3];
		Client *target = getClientByNick(targetNick);
		if (!target)
		{
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::nickNotFound(client, targetNick));
			return;
		}
		set ? channel->addOperator(target) : channel->removeOperator(target);
		channel->sendMessageToClients(MsgFormat::changeOpStatus(client, target, channelName, set));
		return;
	}
	else if (modeFlag == 'l')
	{
		if (3 > vec.size() - 1)
			return;
		if (vec[3].empty())
		{
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidModeParams(client, channelName, mode));
			return;
		}
		for (size_t j = 0; vec[3][j]; j++)
		{
			if (vec[3][j] < '0' || vec[3][j] > '9')
			{
				MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidModeParams(client, channelName, mode));
				return;
			}
		}
		channel->setUserLimit(set ? std::atoi(vec[3].c_str()) : 0);
	}
	else if (modeFlag == 'k')
	{
		if (!set)
			channel->unsetKey();
		else
		{
			if (3 > vec.size() - 1)
				return;
			std::string key = vec[2];
			if (set && key.empty())
			{
				MsgFormat::MsgforHex(client.getSocket(), MsgFormat::invalidModeParams(client, channelName, mode));
				return;
			}

			channel->setKey(key);
			channel->sendMessageToClients(MsgFormat::keySet(client, channelName, key));
			return;
		}
	}
	else
	{
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::unsupportedMode(client, channelName, mode));
		return;
	}
	std::string modeMsg = mode[0] + std::string(1, modeFlag);
	channel->sendMessageToClients(MsgFormat::mode(client, channelName, modeMsg));
}

void ServerManager::validateUser(const std::vector<std::string> &vec, Client &client){

	if(getClientByUser(vec[1]) != NULL){
		MsgFormat::MsgforHex(client.getSocket(), MsgFormat::userAlreadyInUse(vec[1]));
		return;
	}
	client.setName(vec[1]);
}

bool ServerManager::parseArgs(const std::vector<std::string> &vec, Client &client){
	if(vec.empty())
		return false;

	std::map<std::string, std::pair<std::string, size_t> > usageMessage;
	
	usageMessage["NICK"] = std::make_pair("NICK <nickname>, sets your nick", 2);
	usageMessage["USER"] = std::make_pair("USER <username> 0 * :realname, sets your user", 5);
	usageMessage["JOIN"] = std::make_pair("JOIN #<channel> <optional key>, joins the channel", 2);
	usageMessage["PRIVMSG"] = std::make_pair("PRIVMSG <nickname OR channel> :<message>, send a message", 2);
	usageMessage["MODE"] = std::make_pair("MODE #<channel> <+ OR -> <t,i,k,l,o> *for k and l you need a argument here*", 2);
	usageMessage["PART"] = std::make_pair("PART #<channel> :<optional message>, part a channel", 2);
	usageMessage["KICK"] = std::make_pair("KICK #<channel> <nickname> :<optional message> , kick a user from channel", 3);
	usageMessage["INVITE"] = std::make_pair("INVITE <nickname> #<channel>, invite a user to a channel", 3);
	usageMessage["TOPIC"] = std::make_pair("TOPIC #<channel>  <your topic>, sets a topic channel", 2);
	//usageMessage["QUIT"] = std::make_pair("QUIT :<message>, disconect user", 2);
	
	try{
		if (vec.size() < usageMessage.at(vec[0]).second){
			MsgFormat::MsgforHex(client.getSocket(), MsgFormat::usageMsg(vec[0], usageMessage[vec[0]].first));
			return false;
		}
		if (vec[0] == "JOIN")
		{
			if(vec[1][0] != '#' && vec[1][0] != '&'){
				MsgFormat::MsgforHex(client.getSocket(),  MsgFormat::usageMsg(vec[0], usageMessage[vec[0]].first));
				return false;
			}
		}
		else if(vec[0] == "USER"){
			if(vec[2] != "0" || vec[3] != "*" || vec[4] != ":realname")
			{
				MsgFormat::MsgforHex(client.getSocket(), MsgFormat::usageMsg(vec[0], usageMessage[vec[0]].first));
				return false;
			}
		}
	}
	catch (const std::exception &e) {
		return true;
	}
	return true;
}

bool ServerManager::findCmd(const std::vector<std::string> &vec, Client &client, IrcMessages &messages, std::string pass) {
	
	if(!parseArgs(vec, client))
		return true;
	if (vec[0] == "PASS" && (vec.size() > 1))
		handlePass(client, pass, vec[1]);
	else if(client.getAuthenticated())
	{
		if (vec[0] == "NICK")
			changeNick(client, vec[1]);
		else if (vec[0] == "USER") 
			validateUser(vec, client);
		else if(client.checkLoginData()){
			if (vec[0] == "JOIN")
				handleJoinCommand(client, vec[1], vec);
			else if(vec[0] == "PRIVMSG")
				handlePrivMessage(client, vec[1], messages);
			else if(vec[0] == "PART")
				handlePart(client, messages, vec[1]);
			else if(vec[0] == "QUIT")
				return (handleQuit(client, messages._message));
			else if(vec[0] == "KICK")
				handleKick(client, vec[1], vec[2], vec);
			else if(vec[0] == "INVITE")
				handleInvite(client, vec[1], vec[2]);
			else if(vec[0] == "TOPIC")
				handleTopic(client, vec);
			else if(vec[0] == "MODE")
				handleMode(client, vec);
		}
	}
	return true;
}

std::vector<std::string> splitNewLine(std::string buff){
	
	std::string node;
	size_t start = 0;
	std::vector<std::string> result;
	if(buff[0] == '\n' || buff[0] == '\r' || buff[0] == '\0'){
		return result;
	}

	for(size_t i = 0; i < buff.size(); ++i)
	{
		if(buff[i] == '\n'){
			node = buff.substr(start, i - start);
			result.push_back(node);
			start = i + 1;
		}
	}
	return result;
}

void ServerManager::handleIrcCmds(std::string buff, int fd, std::string pass){
	
	for (size_t j = 0; j < _clients.size(); ++j) {
		if (fd == _clients[j]->getSocket()) {
			if(_clients[j]->saveBuffer(buff))
			{
				std::vector<std::string> vecLines = splitNewLine(_clients[j]->getBuffer());
				if(vecLines.empty()){
					_clients[j]->clearBuffer();
				}
				for(size_t i = 0; i < vecLines.size(); ++i)
				{
					IrcMessages message(vecLines[i]);

					if(!findCmd(message._vecMsg, *_clients[j], message, pass))
						return;
					_clients[j]->clearBuffer();
				}
			}
		}
	}
}
