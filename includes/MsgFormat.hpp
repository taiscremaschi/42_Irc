#ifndef MSGFORMAT_HPP
# define MSGFORMAT_HPP

# include "Channel.hpp"
# include <string>

class Channel;
class Client;

class MsgFormat {
    public:
        static std::string join(Client &client, const std::string &channelName);
		static std::string invite(Client &client, const std::string &channelName, const std::string &targetNick);
		static std::string inviteConfirm(Client &client, const std::string &channelName, const std::string &targetNick);
        static std::string topic(Client &client, const std::string &channelName, std::string topic);
        static std::string topicCreator(Client &client, const std::string &channelName);
        static std::string endOfName(Client &client, const std::string &channelName);
        static std::string part(Client &client, Channel *channel, std::string exitMsg);
        static std::string channelNotFound(Client &client, std::string WrongChannel);
        static std::string nickError(Client &client, std::string nick);
        static std::string nick(Client &client, const std::string &oldNick);
        static std::string priv(Client &client, std::string name, std::string message);
        static std::string userNotFound(Client &client, std::string type);
        static void MsgforHex(int clientSocket, const std::string& message); 
        static std::string handleMsg(std::string msg);
        static std::string quit(Client &client, std::string message);
        static std::string notifyNickChanged(Client& client, std::string oldNickname);
        static std::string notifyUserNotInChannel(Client &client, const std::string &channelName);
        static std::string passInvalid();
        static std::string passValid();
        static std::string UserNotAutenticated();
		static std::string notChannelOperator(const Client &client, const std::string &channelName);
		static std::string nickNotFound(Client &client, const std::string &targetNick);
		static std::string userNotInChannel(Client &client, const std::string &channelName, const std::string &targetNick);
		static std::string youNotInChannel(Client &client, const std::string &channelName);
		static std::string userAlreadyInChannel(Client &client, const std::string &channelName, const std::string &targetNick);
		static std::string kickUser(Client &client, const std::string &channelName, const std::string &targetNick, const std::string &reason);
		static std::string mode(Client &client, const std::string &channelName, const std::string &modeMsg);
		static std::string unsupportedMode(Client &client, const std::string &channelName, const std::string &mode);
		static std::string invalidModeParams(Client &client, const std::string &channelName, const std::string &mode);
		static std::string inviteOnlyChannel(Client &client, const std::string &channelName);
		static std::string invalidKey(Client &client, const std::string &channelName);
		static std::string keySet(Client &client, const std::string &channelName, const std::string &key);
		static std::string changeOpStatus(Client &client, Client *target, const std::string &channelName, bool set);
		static std::string channelFull(Client &client, const std::string &channelName);
        static std::string modeactive(const std::string &channelName, std::string mode);
        static std::string userAlreadyInUse(const std::string &username);
        static std::string usageMsg(std::string cmd, std::string usage);
        static std::string limitMsg(std::string nickname, std::string channel, std::string limit);
         
};

#endif