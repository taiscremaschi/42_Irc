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
		bool				_topicOpOnly;
		std::vector<Client*> _invitedClients;
		bool				_keySet;
		std::string			_key;
		int					_userLimit;
		bool				_limitEnabled;
		bool  				_newChannel;

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
		void removeInvite(Client *client);

        std::vector<Client*>  &getOperator();
        bool addClient(Client *client);
        void removeClient(Client *client);
        std::vector<std::string> getAllClientsName();
        std::vector<Client*> getAllClients();
        bool searchOperator(const std::string &name);
		void addOperator(Client *client);
		void removeOperator(Client *client);
        bool searchNames(const std::string name);
        void sendMessageToClients(std::string msg);

		void setTopicOpOnly(bool value);
		bool isTopicOpOnly() const;
		void setKey(const std::string &key);
		void unsetKey(void);
		bool hasKey(void) const;
		bool checkKey(const std::string & key) const;
		void setUserLimit(int limit);
		int getUserLimit(void) const;
		bool isLimitEnabled(void) const;
		int getClientNumber(void) const;
		bool isNew(void) const;
		std::string getModes() const;
		void setNew(bool isnew);

};

#endif


