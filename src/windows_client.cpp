#include "windows_client.h"

bool WS::Initialize(){

    WSADATA wsadata;

    return WSAStartup(MAKEWORD(2, 2), &wsadata) == 0;
}


void WS::CreateSocket(){

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

}

void WS::ConnectWithServer(){
    SOCKADDR_IN server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port_number);

    if (InetPtonA(AF_INET, server_ip_address.c_str(), &server_address.sin_addr) != 1) {
        std::cerr << "Failed to create socket address!";
        closesocket(client_socket);
        WSACleanup();
        exit(-1);
    }

    if (connect(client_socket, reinterpret_cast<SOCKADDR*>(&server_address), sizeof(sockaddr)) != 0) {
        std::cerr << "Failed to connect with server!";
        closesocket(client_socket);
        WSACleanup();
        exit(-1);
    }
}