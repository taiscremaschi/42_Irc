
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "IrcMessages.hpp"
#include <map>


class ServerManager{
    private:
        std::vector<Channel>    _channels;
        std::vector<Client>     _clients;
    public:
        ServerManager();
        ~ServerManager();
        //void        printChannels();
        void createClient(Client &client);
        void removeClient(int i);
        void handleIrcCmds(std::string buff, int fd);
        void findClientBySocket(int socket);
        void handleJoinCommand(Client& client, const std::string& channel);
        void findCmd(const std::vector<std::string> &vec, Client  &client, IrcMessages &mensage);
        std::string channelExists(Client& client, const std::string& channelName);
        void infoForChannel(Client &client, std::string channel);
        bool changeNick(Client &client, const std::string &nick);
        Client *getClientByNick(const std::string &nick);
        Channel *getChannelByNick(const std::string &nick);

};

#endif
