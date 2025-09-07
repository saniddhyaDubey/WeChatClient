#include <iostream>
#include<thread>
#include "windows_client.h"

int main(){
    
    std::cout<<"-------------------------------------------------WELCOME TO WECHAT----------------------------------\n";

    #ifdef _WIN32
    {
        std::cout << "Windows OS\n";

        WS::Initialize();

        WS::CreateSocket();

        WS::ConnectWithServer();

        std::string data_to_send = "Hi Server! Client here";

        unsigned int bytes_sent = WS::SendData(data_to_send);

        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Failed to send the data!";
            closesocket(WS::client_socket);
            WSACleanup();
            exit(-1);
        }

        closesocket(WS::client_socket);
        WSACleanup();

    }
    #else
    {
        std::cout << "Non Windows OS\n";
    }
    #endif

    return 0;

}
