
#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include "Irc.hpp"

class IrcMessages {
private:

public:
    std::string              _message;
    std::vector<std::string> _vecMsg;
    IrcMessages(std::string msg);
    ~IrcMessages();
    std::vector<std::string> splitNewlineAndSpace();
};




# endif