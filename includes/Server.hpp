#ifndef SERVER_HPP
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
        bool checkPass(std::string pass);

        class  createSocket : public std::exception {
            virtual const char* what() const throw(){
                return"error in creating server socket";
            }
        };
        class  conectionError : public std::exception {
            virtual const char* what() const throw(){
                return"error in connection server";
            }
        };

        class errorReuseador : public std::exception {
            virtual const char* what() const throw(){
                return "Error setting SO_REUSEADDR";
            }
        };
        class errorInBind : public std::exception {
            virtual const char* what() const throw(){
                return "error in bind";
            }
         };
        class errorInListen : public std::exception {
            virtual const char* what() const throw(){
                return "error in listen";
            }
         };
};

#endif
