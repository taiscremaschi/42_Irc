
#include "MsgFormat.hpp"

std::string MsgFormat::join(Client &client, const std::string &channelName){
    return (":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " JOIN :" + channelName);
}

std::string MsgFormat::part(Client &client, Channel *channel, std::string exitMsg) {
    return (":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PART " + channel->getName() + " :" + exitMsg);
}

std::string MsgFormat::nick(Client &client, const std::string &oldNick) {
    return (":" + oldNick + " NICK " + client.getNickname()); 
}

std::string MsgFormat::priv(Client &client, std::string name, std::string message) {
    return ":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " PRIVMSG " + name + " :" + message;
}

std::string MsgFormat::quit(Client &client, std::string message){
    return (":" + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " QUIT :" + message);

}

std::string MsgFormat::endOfName(Client &client, const std::string &channelName){
    return (":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.");
}

/// topics
std::string MsgFormat::topic(Client &client, const std::string &channelName, std::string topic) {
    return(":server 332 " + client.getNickname() + " " + channelName + " :" + topic);
}

std::string MsgFormat::topicCreator(Client &client, const std::string &channelName){
    return(":server 333 " + client.getNickname() + " " + channelName + " " + client.getNickname() + "!" + client.getName() + "@" + client.getHostname() + " 0");
}
/// error handling
std::string MsgFormat::nickError(Client &client, std::string nick) {
    return (":server 433 " + client.getNickname() + " " + nick + " :Nickname is already in use");
}

std::string MsgFormat::partError(Client &client, std::string wrongChannel) {
    return (":server 403 " + client.getNickname() + " " + wrongChannel + " :No such channel");
}

std::string MsgFormat::privError(Client &client, std::string type) {
    return (":server 401 " + client.getNickname() + " " + type + " :No such nick/channel");
}

std::string MsgFormat::notifyNickChanged(Client& client, std::string oldNickname) {
    return (":" + oldNickname + "!" + client.getName() + "@" + client.getHostname() + " NICK :" + client.getNickname());
}

std::string MsgFormat::notifyUserNotInChannel(Client &client, const std::string &channelName)
{
    return (":server_name 404 " + client.getNickname() + " " + channelName + " :Cannot send to channel");
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
    //std::cout << msg << std::endl;
    send(clientSocket, msg.c_str(), msg.length(), 0);
}


