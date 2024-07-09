#include "../includes/Server.hpp"

Server::Server() {}

Server::~Server() {}

int Server::_myPortConvertor(char *av)
{
	int i = 0;
	int result = 0;
	while(av[i])
	{
		if(av[i] < 48 || av[i] > 57)
			return -1;
		i++;
	}
	if(i  > 5)
		return -1;
	i = 0;
	while(av[i])
	{
		result = result * 10; 
		result = result + av[i] - '0';
		i++;
	}
	if(result < 1024 || result > 65535)
		return -1;

	return result;
}

int Server::setPort(char *av){

	_port  = _myPortConvertor(av);
	return (_port);
}

void Server::savePass(char *av)
{
	_password = av;
}

std::string	 Server::readData(int i){
	
	char buffer[1024];
	int bytesReceived = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived <= 0) { 
		// Erro ou conexão fechada pelo cliente
		if (bytesReceived == 0) {
			std::cout << "Client disconnected\n";
		} 
		else {
			std::cerr << "Error in recv\n";
		}
		close(_fds[i].fd);
		_fds.erase(_fds.begin() + i);
		_manager.removeClient(i - 1);
		return std::string();
	}
	buffer[bytesReceived] = '\0';
	std::string result = buffer;
	return result;
  
}

void Server::newClientConnection()
{
	// Novo cliente tentando se conectar
	sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(_serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
	if (clientSocket == -1) 
	{
		std::cerr << "Erro in accept conection\n";
		return;
	}
	pollfd client;
	client.fd = clientSocket;
	client.events = POLLIN;
	client.revents = 0;
	_fds.push_back(client);
	Client *newClient = new Client(clientSocket);
	_manager.createClient(newClient);
	std::cout << "new client conected\n";

}

void Server::runServer()
{
	while (1) {
		int result = poll(_fds.data(), _fds.size(), -1);
		if (result < 0) {
			std::cerr << "Error in poll()\n";
			break;
		}
		for (size_t i = 0; i < _fds.size(); ++i) {
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _serverSocket) 
					newClientConnection();
				else {
					// Cliente existente enviando dados
					std::string buff = readData(i);
					if(buff.empty())
						continue ;
					std::cout << "Received: " << buff << std::endl;
					_manager.handleIrcCmds(buff, _fds[i].fd, this->_password);
				}
			}
			else if (_fds[i].revents & POLLNVAL)
			{
				_fds.erase(_fds.begin() + i);
			}
		}
	}
}

void Server::createServerSocket()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
	if(_serverSocket == -1){
		std::cerr << "error in creating server socket\n";
		return;
	}

	if(fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
		return;
 	// Definir a opção SO_REUSEADDR no socket do servidor
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Error setting SO_REUSEADDR\n";
		close(_serverSocket);
		return;
	}
	sockaddr_in serverAddr; // now I need configure the server address (read help 2)
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_addr.s_addr = INADDR_ANY; 
	serverAddr.sin_port = htons(_port); 

	if(bind(_serverSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
		close(_serverSocket);
		std::cerr << "error in bind" << std::endl; //help 3
		return;
	}
	if(listen(_serverSocket, 5) == -1) {
		std::cerr << "error in listen\n"; // max_nbr of connections
		close(_serverSocket);
		return;
	}
}

void Server::inicializeServer() {
	
	createServerSocket();
	std::cout << "Server waiting for connections...\n";
	pollfd server;
	server.fd = _serverSocket;
	server.events = POLLIN; 
	_fds.push_back(server); //add the server socket to the set of descriptors to be monitored by poll()
	runServer();
	close(_serverSocket);
}


