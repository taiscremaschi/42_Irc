
#include "IrcMessages.hpp"

IrcMessages::IrcMessages(std::string msg)
{
    _message = msg;
}

IrcMessages::~IrcMessages(){

}

std::vector<std::string> IrcMessages::split(){
    
    size_t i = 0;
    std::vector<std::string> result;
    std::string msg;
    int start = 0;
    while(i < _message.size())
    {
        while(_message[i] == ' ' || _message[i] == '\n')
            i++;
        start = i;
        while(_message[i] != '\0' && (_message[i] != ' ' && _message[i] != '\n'))
            i++;
        msg = _message.substr(start, i - start);
        result.push_back(msg);
        i++;
    }
    return result;
}



// void findCmd(std::vector<std::string> vec)
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