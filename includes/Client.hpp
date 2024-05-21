
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <iostream>
#include <netdb.h>

class Client {
protected:
    std::string             _name;
    std::string             _nickname;
    int                     _socketClient;
    std::string             _hostname;

public:
    Client(int clientSocket);
    ~Client();
    int getSocketClient();
    void setSocketClient(int socketClient);
    void setName(std::string name);
    void setNickname(std::string nickname);
    std::string getName() const;
    std::string getNickname() const;
    std::string getHostname();

};

#endif

    // Client(int socketClient) : _socketClient(socketClient) {}
    // ~Client() { close(_socketClient); }
 //chat melhorias
 
