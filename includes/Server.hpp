
#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Server {
private:
int                     _serverSocket;
int                     _port;
std::string             _password;
std::vector<Client>     _clients;
std::vector<pollfd>     _fds;
int                     _myPortConvertor(char *av);


public:

Server();
~Server();
void    inicializeServer();
int     setPort(char *av);
void    savePass(char *av);
void    runServer();
void    newClientConnection();
void    readData(int i);

};

#endif


// Server::Server() : _serverSocket(-1), _port(0) {}

// Server::~Server() {
//     if (_serverSocket != -1) {
//         close(_serverSocket);
//     }
//     for (size_t i = 0; i &lt; _clients.size(); ++i) {
//         close(_clients[i]._socketClient);
//     }
// }