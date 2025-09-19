#pragma once
#include<iostream>
#include "config.h"
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<tchar.h>
#pragma comment(lib,"ws2_32.lib")

namespace WS{

    extern SOCKET client_socket;

    void Initialize(); // initialize winsock

    void CreateSocket(); // create socket

    void ConnectWithServer(); // connect with server

    int SendData(std::string user,std::string data); // send data

    std::string RecieveData();
}