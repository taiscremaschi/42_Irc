

#ifndef MSGFORMATIRC_HPP
#define MSGFORMATIRC_HPP

#include <string>
#include <Channel.hpp>

class MsgFormatIrc {
    public:
    static std::string joinMessage(Client &client, const std::string &channelName);
    static std::string topicMessage(Client &client, const std::string &channelName, std::string topic);
    static std::string topicCreatorMessage(Client &client, const std::string &channelName);
    static std::string endOfNameMessage(Client &client, const std::string &channelName);
    static std::string partMessage(Client &client, Channel *channel, std::string &exitMsg);
    static std::string partErrorMessage(Client &client, std::string WrongChannel);
    static std::string nickErrorMessage(Client &client, std::string nick);
    static std::string nickMessage(Client &client, const std::string &oldNick);
    static std::string privMessage(Client &client, Client *receiver, std::string cmd);
    static std::string privMessage(Client &client, Channel *channel, std::string message);
    static std::string privErrorMessage(Client &client, std::string type);



};


#endif