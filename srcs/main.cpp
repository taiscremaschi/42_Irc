#include "../includes/Irc.hpp"
#include "../includes/Server.hpp"
#include <signal.h>

#define MAX_FDS 1024

void handleSig(int sig)
{
	(void) sig;
	for(int i = 0; i < MAX_FDS; ++i)
		close(i);
}

int errorMsg(std::string msg){
	std::cout << msg << std::endl;
	return 1;
}

int main(int ac, char **av)
{
	Server serverClass;
	
	signal(SIGINT, handleSig);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT,handleSig);
	if(ac != 3)
		return (errorMsg("Wrong number of arguments"));
	if(serverClass.setPort(av[1]) == -1)
		return (errorMsg("Invalid port number"));
	if(!serverClass.savePass(av[2]))
		return (errorMsg("Invalid pass"));
	serverClass.inicializeServer();
	return 0;
}
