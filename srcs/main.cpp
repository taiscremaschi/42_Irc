
#include "Irc.hpp"

int main(int ac, char **av)
{
    Server serverClass;

    if(ac != 3)
        return (errorMsg("Wrong number of arguments"));

    if(serverClass.setPort(av[1]) == -1)
        return (errorMsg("Invalid port number"));
    // TODO: tratar das passwords aqui nessa linha
    serverClass.inicializeServer();
    
    return 0;
}
