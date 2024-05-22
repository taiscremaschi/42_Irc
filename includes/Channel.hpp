
#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include "Client.hpp"
#include <vector>



class Channel {
    private:
    std::vector<Client> _clientsChannel;
    std::string         _name;
    std::string         _topic;
    std::vector<Client> _operators;
    public:
        Channel(const std::string &name, const Client &client);
        ~Channel();
        const std::string &getName() const;
        void addClient(Client &client);
};


#endif


