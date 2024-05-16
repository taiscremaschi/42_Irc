#include "Irc.hpp"

#ifndef SERVER_HPP
# define SERVER_HPP


class Server {
private:
int _serverSocket;
int _port;
int _myPortConvertor(char *av);
public:

Server();
~Server();
void inicializeServer();
int setPort(char *av);

};


#endif