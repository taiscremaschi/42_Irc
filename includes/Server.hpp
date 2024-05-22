
# ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "Channel.hpp"

class Client;

class Server {
private:
int                     _serverSocket;
int                     _port;
std::string             _password;
std::vector<Client>     _clients;
std::vector<pollfd>     _fds;
//std::vector<Channel>    _channels;
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
void handleJoinCommand(Client& client, const std::string& channel);
void findCmd(const std::vector<std::string> &vec, Client* client, int clientSocket);

};

#endif

