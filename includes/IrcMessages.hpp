
#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include "Irc.hpp"

class IrcMessages {
private:
    std::string              _message;
    std::vector<std::string> _vecMsg;

public:
    IrcMessages(std::string msg);
    ~IrcMessages();
    std::vector<std::string> split(char c);
    void findCmd(std::vector<std::string> vec, Client *client);






};




# endif