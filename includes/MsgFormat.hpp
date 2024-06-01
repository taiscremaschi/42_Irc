
#ifndef MSGFORMAT_HPP
#define MSGFORMAT_HPP

#include <string>

class Channel;
class Client;

class MsgFormat {
    public:
        static std::string join(Client &client, const std::string &channelName);
        static std::string topic(Client &client, const std::string &channelName, std::string topic);
        static std::string topicCreator(Client &client, const std::string &channelName);
        static std::string endOfName(Client &client, const std::string &channelName);
        static std::string part(Client &client, Channel *channel, std::string exitMsg);
        static std::string partError(Client &client, std::string WrongChannel);
        static std::string nickError(Client &client, std::string nick);
        static std::string nick(Client &client, const std::string &oldNick);
        static std::string priv(Client &client, std::string name, std::string message);
        static std::string privError(Client &client, std::string type);
        static void MsgforHex(int clientSocket, const std::string& message); 
        static std::string handleMsg(std::string msg);
        static std::string quit(Client &client, std::string message);
        static std::string notifyNickChanged(Client& client, std::string oldNickname);
        static std::string notifyUserNotInChannel(Client &client, const std::string &channelName);
        static std::string passInvalid();
        static std::string passValid();
        static std::string UserNotAutenticated();

};

#include "Channel.hpp"

#endif