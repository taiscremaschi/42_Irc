#include <iostream>
#include <string>
#include <cassert>

class IrcClient {
    public:
        IrcClient(std::string nick);
        ~IrcClient();

    //Method
        void    connect(std::string address, int port);
        bool    pass(std::string password);
        void    join(std::string group);
        void    invite(std::string user, std::string group);
        void    mensage(std::string msg);
        void    disconnect();

    private:
};

int main()
{
    //server tem que estar rodando
    IrcClient   renan("renan");
    IrcClient   paula("paula");
    std::string address = "127.0.0.1";
    int         port = 6667;
    std::string pass = "123";
    std::string wrongPass = "111";


    //conectar
    renan.connect(address, port);
    paula.connect(address, port);

    //pass
    assert(renan.pass(wrongPass) == false);
    assert(renan.pass(pass));
    assert(paula.pass(pass));

    //join grupo
    paula.join("#teste");
    renan.join("#teste");

    //enviar msg
    paula.mensage("hello!");
    renan.mensage("How are you?");
    
    return 0;
}
