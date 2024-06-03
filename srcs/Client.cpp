#include "../includes/Client.hpp"

Client::Client(int clientSocket) {
   _socketClient =  clientSocket;
   _authenticated = false;
}

Client:: ~Client(){
}

bool Client::operator==(Client const &client){
	if(this == &client)
		return true;
	return false;
}

int Client::getSocket(){
	return _socketClient;
}

void Client::setSocket(int socketClient){
	_socketClient = socketClient;
}

void Client::setName(std::string name){
	_name = name;
}

std::string Client::getName() const{
	return _name;
}

void Client::setNickname(std::string nick){
	_nickname = nick;
}

std::string Client::getNickname() const{
	return _nickname;
}

std::string Client::getHostname() { //help 4
	char buff[1024];
	sockaddr_storage addr;
	socklen_t addr_len = sizeof(addr);

	if (getpeername(_socketClient, (sockaddr*)&addr, &addr_len) == -1) { //funcao pra obter o ip e a porta do cliente. paramentris, o fd o endereli para armazenar o add, e o size da estrutura
		std:: cerr << "Error in getting IP address" << std::endl;
		return "";
	}

	int result = getnameinfo((sockaddr*)&addr, addr_len, buff, sizeof(buff), NULL, 0, NI_NAMEREQD); // funtion to convert ip address into hostname. paramenters: endereço, size do address, buff para armazenar o host, size do buff, flag que idica a necesdidadedo hostname 
	if (result != 0) {
		std::cerr << "Error in convert IP address to hostname" << std::endl;
		return "";
	}
	return std::string(buff);
}

void Client::isAuthenticated() {
	_authenticated = true;
}

bool Client::getAuthenticated() const{
	if(!_authenticated)
		MsgFormat::MsgforHex(_socketClient, MsgFormat::UserNotAutenticated());
	return _authenticated;
}

bool Client::checkLoginData(){
	if( _name.empty() || _nickname.empty())
	{
		MsgFormat::MsgforHex(_socketClient, MsgFormat::UserNotAutenticated());
		return false;
	}
	return true;
}

bool Client::saveBuffer(std::string buff)
{
	_bufferMsg += buff;
	return (buff[buff.size() - 1] == '\n');
}

std::string Client::getBuffer(){
	return _bufferMsg;
}

void Client::clearBuffer(){
	_bufferMsg.clear();
}
