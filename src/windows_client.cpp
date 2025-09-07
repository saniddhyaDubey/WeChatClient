#include "windows_client.h"


SOCKET WS::client_socket                = INVALID_SOCKET;
std::string WS::server_ip_address       = "10.148.29.31";
unsigned int WS::server_port_number     = 12345;

void WS::Initialize(){

    WSADATA wsadata;

    if(WSAStartup(MAKEWORD(2, 2), &wsadata) !=0){
        std::cerr << "Failed to create socket address!";
        exit(-1);
    }
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


int WS::SendData(std::string data){
    return send(client_socket, data.c_str(), data.length(), 0);
}