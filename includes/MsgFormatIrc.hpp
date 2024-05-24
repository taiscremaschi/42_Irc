

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





    static std::string partMessage(Client &client, std::string cmd, Channel &channel);
    static std::string nickMessage(Client &client, std::string cmd);
    static std::string privMessage(Client &client, std::string cmd);



};


#endif