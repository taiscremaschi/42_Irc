
# ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "ServerManager.hpp"

class Server {
    private:
        ServerManager           _manager;
        std::string             _password;
        std::vector<pollfd>     _fds;
        int                     _serverSocket;
        int                     _port;
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
        std::string getPass(std::string pass, std::string check);

};

#endif

