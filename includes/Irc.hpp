
#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <cstdio>
#include "Server.hpp"

int errorMsg(std::string msg);


#endif