
#include "MsgFormatIrc.hpp"


std::string MsgFormatIrc::joinMessage(Client &client, const std::string &channelName){

    return (":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName);

}

std::string MsgFormatIrc::partMessage(Client &client, Channel *channel, std::string &exitMsg)
{
    return (":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PART " + channel->getName() + " :" + exitMsg);
}

std::string MsgFormatIrc::nickMessage(Client &client, const std::string &oldNick)
{
    return (":" + oldNick + " NICK " + client.getNickname()); 
}

std::string MsgFormatIrc::privMessage(Client &client, std::string name, std::string message)
{
    return ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PRIVMSG " + name + " :" + message;
}

std::string MsgFormatIrc::endOfNameMessage(Client &client, const std::string &channelName){
    return (":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.");
}

/// topics
std::string MsgFormatIrc::topicMessage(Client &client, const std::string &channelName, std::string topic) {
    return(":server 332 " + client.getNickname() + " " + channelName + " :" + topic);
}

std::string MsgFormatIrc::topicCreatorMessage(Client &client, const std::string &channelName){
    return(":server 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " 0");
}
/// error handling
std::string MsgFormatIrc::nickErrorMessage(Client &client, std::string nick)
{
    return (":server 433 " + client.getNickname() + " " + nick + " :Nickname is already in use");
}

std::string MsgFormatIrc::partErrorMessage(Client &client, std::string wrongChannel)
{
    return (":server 403 " + client.getNickname() + " " + wrongChannel + " :No such channel");
}

std::string MsgFormatIrc::privErrorMessage(Client &client, std::string type)
{
    return (":server 401 " + client.getNickname() + " " + type + " :No such nick/channel");
}
