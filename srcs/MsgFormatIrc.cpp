
#include "MsgFormatIrc.hpp"


std::string MsgFormatIrc::joinMessage(Client &client, const std::string &channelName){

    return (":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName);

}

std::string MsgFormatIrc::topicMessage(Client &client, const std::string &channelName, std::string topic) {
    return(":server 332 " + client.getNickname() + " " + channelName + " :" + topic);
}

std::string MsgFormatIrc::topicCreatorMessage(Client &client, const std::string &channelName){
    return(":server 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " 0");
}

std::string MsgFormatIrc::endOfNameMessage(Client &client, const std::string &channelName){
    return (":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.");
}


std::string MsgFormatIrc::partMessage(Client &client, std::string cmd, Channel &channel)
{
    return ;
}

std::string MsgFormatIrc::nickMessage(Client &client, std::string cmd)
{
    return ; 
}

std::string MsgFormatIrc::privMessage(Client &client, std::string cmd)
{
    return ;
}


