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
