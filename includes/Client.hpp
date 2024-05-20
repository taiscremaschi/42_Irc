
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>

class Client {
protected:
    std::string     _name;
    std::string     _nickname;
    int             _socketClient;
public:
    Client(int clientSocket);
    ~Client();
    int getSocketClient();
    void setSocketClient(int socketClient);
    void setName(std::string name);
    void setNickname(std::string nickname);
    std::string getName() const;
    std::string getNickname() const;

};

#endif

    // Client(int socketClient) : _socketClient(socketClient) {}
    // ~Client() { close(_socketClient); }
 //chat melhorias
 
