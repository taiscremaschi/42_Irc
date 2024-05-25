
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "MsgFormatIrc.hpp"
#include "IrcMessages.hpp"
#include <map>


class ServerManager{
    private:
        std::vector<Channel>    _channels;
        std::vector<Client>     _clients;
    public:
        ServerManager();
        ~ServerManager();
        void createClient(Client &client);
        void removeClient(int i);
        void handleIrcCmds(std::string buff, int fd);
        void handleJoinCommand(Client& client, const std::string& channel);
        void findCmd(const std::vector<std::string> &vec, Client  &client, IrcMessages &mensage);
        void infoForChannel(Client &client, std::string channel);
        bool changeNick(Client &client, const std::string &nick);
        Client *getClientByNick(const std::string &nick);
        Channel *getChannelByName(const std::string &nick);
        void handlePrivMessage(Client& client, const std::string& type, IrcMessages &messages);
        void handlePart(Client& client, IrcMessages &messages,const std::string& channelName);

        

};

# endif
