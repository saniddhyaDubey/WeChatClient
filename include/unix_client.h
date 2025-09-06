#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h> 
#pragma once    //To avoid the Redefinition error

namespace US {
    extern int client_socket;           //extern is avoiding definition of the variable here
    extern sockaddr_in server_info;

    void createSocket(int, int, int);   //create a socket for client!
    void connectToServer();             //connect with the server!
};
