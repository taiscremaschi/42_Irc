
#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include "Irc.hpp"

class Channel {
    private:
    std::vector<Client> _clientsChannel;
    std::string         _channelName;
    public:
        Channel();
        ~Channel();
};


#endif


