
#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include "Irc.hpp"

class IrcMessages {
private:
    std::string              _message;

public:
    std::vector<std::string> _vecMsg;
    IrcMessages(std::string msg);
    ~IrcMessages();
    std::vector<std::string> split(char c);
};




# endif