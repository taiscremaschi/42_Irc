#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include "IrcMessages.hpp"

class ServerManager{
    private:
        std::vector<Channel*>    _channels;
        std::vector<Client*>     _clients;
    public:
        ServerManager();
        ~ServerManager();
        void createClient(Client *client);
        void handleIrcCmds(std::string buff, int fd, std::string pass);
        void handleJoinCommand(Client& client, const std::string& channel, const std::string &key);
        bool findCmd(const std::vector<std::string> &vec, Client  &client, IrcMessages &, std::string pass);
        bool changeNick(Client &client, const std::string &nick);
        Client *getClientByNick(const std::string &nick);
        Client *getClientByUser(const std::string &user);
        Channel *getChannelByName(const std::string &nick);
        void handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages);
        void handlePart(Client& client, IrcMessages &messages,const std::string& channelName);
        void handleQuit(Client& client, const std::string &quitMsg);
		void handleKick(Client &client, const std::string &channelName, const std::string &targetNick, const std::vector<std::string> &vec, size_t i);
		void handleTopic(Client &client, const std::vector<std::string> &vec, size_t i);
		void handleInvite(Client &client, const std::string &targetNick, const std::string &channelName);
		void handleMode(Client &client, std::vector<std::string> vec, size_t i);
        void removeClientByNick(std::string nick);
        void removeClient(int i);
        void removeClientByFd(int fd);
        bool handlePass(Client& client, std::string pass, std::string vec);
        bool validateUser(const std::vector<std::string> &vec, Client &client, size_t i);


};

# endif
