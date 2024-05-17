
#include "IrcMessages.hpp"

IrcMessages::IrcMessages(std::string msg)
{
    _message = msg;
    _vecMsg = split('\n');
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




// void IrcMessages::findCmd(std::vector<std::string> vec, Client *client)
// {
//     for(size_t i = 0; i < vec.size(); i++)
//     {
//         if(vec[i] == "NICK")
//         {
//             _nickname = vec[i + 1];

//         }
//         if(vec[i] == "USER")
//         {

//         }

//     }

// }

