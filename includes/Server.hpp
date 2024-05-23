
# ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "ServerManager.hpp"

class Server {
private:
int                     _serverSocket;
int                     _port;
std::string             _password;
std::vector<pollfd>     _fds;
ServerManager           _manager;
int                     _myPortConvertor(char *av);

public:

Server();
~Server();
void        inicializeServer();
int         setPort(char *av);
void        savePass(char *av);
void        runServer();
void        newClientConnection();
std::string readData(int i);
void        createServerSocket();



};

#endif

