#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <iostream>
#include <netdb.h>

class MsgFormat;

class Client {

    private:
    
        std::string             _name;
        std::string             _nickname;
        int                     _socketClient;
        std::string             _hostname;
        bool                    _authenticated;
        std::string             _bufferMsg;
        bool                    _nickError;

    public:
        Client(int clientSocket);
        ~Client();
        int getSocket();
        void setSocket(int socketClient);
        void setName(std::string name);
        void setNickname(std::string nickname);
        std::string getName() const;
        std::string getNickname() const;
        bool operator==(Client const &client);
        void isAuthenticated();
        bool getAuthenticated() const;
        bool checkLoginData();
        bool saveBuffer(std::string buff);
        std::string getBuffer();
        void clearBuffer();
        bool getNickError();
        void setNickError(bool nickError);

};

#include "MsgFormat.hpp"

#endif
