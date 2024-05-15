

#ifndef IRC_HPP
#define IRC_HPP


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>


int errorMsg(std::string msg);
int myPortConvertor(char *av);


#endif