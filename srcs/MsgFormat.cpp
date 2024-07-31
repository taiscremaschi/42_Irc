#include "../includes/MsgFormat.hpp"

std::string MsgFormat::join(Client &client, const std::string &channelName){
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost JOIN :" + channelName);
}

std::string MsgFormat::invite(Client &client, const std::string &channelName, const std::string &targetNick)
{
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost INVITE " + targetNick + ":" + channelName);
}

std::string MsgFormat::inviteConfirm(Client &client, const std::string &channelName, const std::string &targetNick)
{
	return ("server 341 " + client.getNickname() + " " + targetNick + ":" + channelName);
}

std::string MsgFormat::part(Client &client, Channel *channel, std::string exitMsg) {
	if(exitMsg[exitMsg.size()-1] == '\n')
		exitMsg.erase(exitMsg.size() - 1, 1);
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost PART " + channel->getName() + " :" + exitMsg);
}

std::string MsgFormat::nick(Client &client, const std::string &oldNick) {
	return (":" + oldNick + " NICK " + client.getNickname()); 
}

std::string MsgFormat::priv(Client &client, std::string name, std::string message) {
	if(message[message.size()-1] == '\n')
		message.erase(message.size() - 1, 1);
	return ":" + client.getNickname() + "!" + client.getName() + "@localhost PRIVMSG " + name + " :" + message;
}

std::string MsgFormat::quit(Client &client, std::string message){
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost QUIT :" + message);

}

std::string MsgFormat::endOfName(Client &client, const std::string &channelName){
	return (":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.");
}

/// topics
std::string MsgFormat::topic(Client &client, const std::string &channelName, std::string topic) {
	return(":server 332 " + client.getNickname() + " " + channelName + " :" + topic);
}

std::string MsgFormat::topicCreator(Client &client, const std::string &channelName){
	return(":server 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@localhost 0");
}
/// error handling
std::string MsgFormat::nickError(Client &client, std::string nick) {
	std::string myNick = client.getNickname();
	if (myNick.empty())
		myNick = "*";
	return (":server 433 " + myNick + " " + nick + " :Nickname is already in use");
}

std::string MsgFormat::channelNotFound(Client &client, std::string wrongChannel) {
	return (":server 403 " + client.getNickname() + " " + wrongChannel + " :No such channel");
}

std::string MsgFormat::notifyNickChanged(Client& client, std::string oldNickname) {
	return (":" + oldNickname + "!" + client.getName() + "@localhost NICK :" + client.getNickname());
}

std::string MsgFormat::notifyUserNotInChannel(Client &client, const std::string &channelName)
{
	return (":server 404 " + client.getNickname() + " " + channelName + " :Cannot send to channel");
}

std::string MsgFormat::passInvalid()
{
	return (":server 464 * :Password incorrect");
}

std::string MsgFormat::passValid()
{
	return (":server NOTICE * :Authentication successful");
}

std::string MsgFormat::UserNotAutenticated()
{
	return (":server 451 * :You have not registered");
}

std::string MsgFormat::notChannelOperator(const Client &client, const std::string &channelName)
{
	return (":server 482 " + client.getNickname() + " " + channelName + " :You're not channel operator");
}

std::string MsgFormat::nickNotFound(Client &client, const std::string &targetNick)
{
	return (":server 401 " + client.getNickname() + " " + targetNick + " :No such nick/channel");
}

std::string MsgFormat::userNotInChannel(Client &client, const std::string &channelName, const std::string &targetNick)
{
	return (":server 441 " + client.getNickname() + " " + targetNick + " "+ channelName + " :User not in channel");
}

std::string MsgFormat::youNotInChannel(Client &client, const std::string &channelName)
{
	return (":server 442 " + client.getNickname() + " " + channelName + " :You're not on that channel");
}

std::string MsgFormat::userAlreadyInChannel(Client &client, const std::string &channelName, const std::string &targetNick)
{
	return (":server 443 " + client.getNickname() + " " + targetNick + " " + channelName + " :is already on channel");
}

std::string MsgFormat::kickUser(Client &client, const std::string &channelName, const std::string &targetNick, const std::string &reason)
{
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost KICK " + channelName + " " + targetNick + " :" + reason);
}

std::string MsgFormat::mode(Client &client, const std::string &channelName, const std::string &modeMsg)
{
	return (":" + client.getNickname() + " MODE " + channelName + " " + modeMsg);
}

std::string MsgFormat::unsupportedMode(Client &client, const std::string &channelName, const std::string &mode)
{
	return (":server 501 " + client.getNickname() + " " + channelName + " :" + mode +" is an unsupported mode");
}

std::string MsgFormat::invalidModeParams(Client &client, const std::string &channelName, const std::string &mode)
{
	return (":server 472 " + client.getNickname() + " " + channelName + " :" + mode +" is an unknown mode character to me");
}

std::string MsgFormat::inviteOnlyChannel(Client &client, const std::string &channelName)
{
	return (":" + client.getNickname() + " 473 " + channelName + " :Cannot join channel (+i)");
}

std::string MsgFormat::invalidKey(Client &client, const std::string &channelName)
{
	return (":server 475 " + client.getNickname() + " " + channelName + " :Cannot join channel (+k) - incorrect key");
}

std::string MsgFormat::keySet(Client &client, const std::string &channelName, const std::string &key)
{
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost MODE " + channelName + " +k " + key);
}

std::string MsgFormat::changeOpStatus(Client &client, Client *target, const std::string &channelName, bool set)
{
	std::string action = set ? "gives" : "removes";
	std::string mode = set ? "+o" : "-o";
	return (":" + client.getNickname() + "!" + client.getName() + "@localhost MODE " + channelName + " " + mode + " " + target->getNickname());
}

std::string MsgFormat::channelFull(Client &client, const std::string &channelName)
{
	return (":server 471 " + client.getNickname() + " " + channelName + " :Cannot join channel (+l)");
}

std::string MsgFormat::userAlreadyInUse(const std::string &username){
	return(":server 400 " + username + " :Username is already in use");
}


std::string  MsgFormat::usageMsg(std::string cmd, std::string usage){
	return(":server 461 " + cmd + " :Usage: " + usage);
}

std::string  MsgFormat::limitMsg(std::string nickname, std::string channel, std::string limit){

	return(":" + nickname + " NOTICE " + channel + " :The user limit for this channel has been set to " + limit + ".");
}


std::string  MsgFormat::ulimitMsg(std::string nickname, std::string channel){

	return(":" + nickname + " NOTICE " + channel + " :The user limit for this channel has been removed.");
}


std::string MsgFormat::handleMsg(std::string msg)
{
	int i = 0;
	while(msg[i])
	{
		if(msg[i] == ':' && msg[i - 1] == ' ' )
			break;
		i++;
	}
	if(msg[i])
		i++;
	return msg.substr(i, msg.length() - i);
}


void MsgFormat::MsgforHex(int clientSocket, const std::string& message) 
{
	std::string msg = message + "\r\n";
	std::cout << msg << std::endl;
	int bytes = send(clientSocket, msg.c_str(), msg.length(), 0);
	if (bytes == -1)
		std::cerr << "send message failed" << std::endl;
}

