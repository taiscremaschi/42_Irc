
#include "IrcMessages.hpp"

IrcMessages::IrcMessages(std::string msg)
{
    _message = msg;
    _vecMsg = split('\n');
    _vecMsg = split(' ');
}

IrcMessages::~IrcMessages(){

}

std::vector<std::string> IrcMessages::split(char c){
    
    size_t i = 0;
    std::string msg;
    std::vector<std::string> result;
    int start = 0;
    while(i < _message.size())
    {
        while(_message[i] == c)
            i++;
        start = i;
        while(_message[i] != '\0' &&  _message[i] != c)
            i++;
        msg = _message.substr(start, i - start);
        result.push_back(msg);
        i++;
    }
    return result;
}





