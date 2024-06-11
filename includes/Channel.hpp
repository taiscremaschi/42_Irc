#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <algorithm>

class Channel {
    private:
        std::vector<Client*> _clientsChannel;
        std::string         _name;
        std::string         _topic;
        std::vector<Client*> _operators;
		bool				_inviteOnly;
		std::vector<Client*> _invitedClients;
    public:
        Channel(const std::string &name, Client *client);
        ~Channel();

        const std::string &getName() const;
		const std::string &getTopic() const;
		void setTopic(const std::string &topic);
		bool isInviteOnly() const;
		void setInviteOnly(bool inviteOnly);
		bool isInvited(Client *client) const;
		void addInvite(Client *client);

        std::vector<Client*>  &getOperator();
        bool addClient(Client *client);
        void removeClient(Client *client);
        std::vector<std::string> getAllClientsName();
        std::vector<Client*> getAllClients();
        bool searchOperator(const std::string &name);
		void removeOperator(Client *client);
        bool searchNames(const std::string name);
        void sendMessageToClients(std::string msg);
};

#endif


