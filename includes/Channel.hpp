
#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include "Irc.hpp"

class Channel {
    private:
    std::vector<Client> _clientsChannel;
    
    public:
        Channel();
        ~Channel();
};


#endif


