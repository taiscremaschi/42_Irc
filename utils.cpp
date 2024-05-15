
#include "Irc.hpp"

int errorMsg(std::string msg)
{
    std::cout << msg << std::endl;
    return 1;
}

int myPortConvertor(char *av)
{
    int i = 0;
    int result = 0;
    while(av[i])
    {
        if(av[i] < 30 || av[i] > 39)
            return -1;
        i++;
    }
    if(i  > 5)
        return -1;
    while(av[i])
    {
        result = result * 10; 
        result = result + av[i] - '0';
        i++;
    }
    if(result < 1024 || result > 65535)
        return -1;

    return result;
}