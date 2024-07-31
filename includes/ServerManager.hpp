#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include "IrcMessages.hpp"
#include <string>
#include <map>
#include <utility> 

class ServerManager{
    private:
        std::vector<Channel*>    _channels;
        std::vector<Client*>     _clients;

    public:
        ServerManager();
        ~ServerManager();
        void createClient(Client *client);
        void handleIrcCmds(std::string buff, int fd, std::string pass);
        void handleJoinCommand(Client& client, const std::string& channel, const std::vector<std::string> &vec);
        bool findCmd(const std::vector<std::string> &vec, Client  &client, IrcMessages &, std::string pass);
        bool changeNick(Client &client, const std::string &nick);
        Client *getClientByNick(const std::string &nick);
        Client *getClientByUser(const std::string &user);
        Channel *getChannelByName(const std::string &nick);
        void handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages);
        void handlePart(Client& client, IrcMessages &messages,const std::string& channelName);
        bool handleQuit(Client& client, const std::string &quitMsg);
		void handleKick(Client &client, const std::string &channelName, const std::string &targetNick, const std::vector<std::string> &vec);
		void handleTopic(Client &client, const std::vector<std::string> &vec);
		void handleInvite(Client &client, const std::string &targetNick, const std::string &channelName);
		void handleMode(Client &client, std::vector<std::string> vec);
        void removeClientByNick(std::string nick);
        void removeClient(int i);
        void removeClientByFd(int fd);
        void handlePass(Client& client, std::string pass, std::string vec);
        void validateUser(const std::vector<std::string> &vec, Client &client);
        bool parseArgs(const std::vector<std::string> &vec, Client &client);



};

# endif
