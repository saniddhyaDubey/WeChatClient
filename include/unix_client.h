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

    void createSocket();                //create a socket for client!
    void connectToServer();             //connect server!
    int sendData(std::string data, std::string recipient_user);     //send data!
    std::vector<std::string> receiveData();
};
