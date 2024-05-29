
#include "Irc.hpp"
#include "Server.hpp"
#include <signal.h>


bool server_shutdown = false;

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		server_shutdown = true;
		std::cout << std::endl;
	}
}

int errorMsg(std::string msg){
    std::cout << msg << std::endl;
    return 1;
}

int main(int ac, char **av)
{
    Server serverClass;

    signal(SIGINT, signalHandler);
    if(ac != 3)
        return (errorMsg("Wrong number of arguments"));

    if(serverClass.setPort(av[1]) == -1)
        return (errorMsg("Invalid port number"));
    serverClass.savePass(av[2]);    
    serverClass.inicializeServer();
    return 0;
}
