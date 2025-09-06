#pragma once
#include<iostream>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<thread>
#include<tchar.h>
#pragma comment(lib,"ws2_32.lib")

namespace WS{

    SOCKET client_socket;

    std::string server_ip_address = "127.0.0.1";

    unsigned int server_port_number      = 12345;

    bool Initialize(); // initialize winsock

    void CreateSocket(); // create socket

    void ConnectWithServer(); // connect with server


}