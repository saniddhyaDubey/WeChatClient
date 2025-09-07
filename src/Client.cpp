#include <iostream>
#include<thread>

#ifdef _WIN32
    #include "windows_client.h"
#elif __APPLE__
    #include "unix_client.h"
#endif

int main(){
    std::cout<<"-------------------------------------------------WELCOME TO WECHAT----------------------------------\n";

    #ifdef _WIN32
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
    #elif __APPLE__
        std::cout << "UNIX OS\n";
        US::createSocket(AF_INET, SOCK_STREAM, 0);
        US::connectToServer();

        std::string data_to_send = "Hey server, this is apple client!";

        int send_response = US::sendData(data_to_send);

        if (send_response == -1){
            std::cerr << "Failed to send the data!";
            close(US::client_socket);
            //do we need to terminate the program or give any other option ?   
        }
    #endif

    return 0;
}
