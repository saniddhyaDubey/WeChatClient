#include <iostream>
#include<thread>
#include<string>

#ifdef _WIN32
    #include "windows_client.h"

    void SendInformation(SOCKET &client_socket){

        std::string user_input;

        while(true){

            std::getline(std::cin, user_input);

            if(user_input=="quit");
            
        }

    }    

#elif __APPLE__
    #include "unix_client.h"
#endif

static std::string rule_book = "Instructions: /register : Register  /login : Login  /switch username : switch chat   /quit : Exit\n"; 

int main(){
    std::cout<<"-------------------------------------------------WELCOME TO WECHAT-----------------------------------------\n";

    #ifdef _WIN32
        std::cout <<rule_book;

        WS::Initialize();

        WS::CreateSocket();

        WS::ConnectWithServer();

        std::string data_to_send = "Hi Server! Client here";

        // unsigned int bytes_sent = WS::SendData(data_to_send);

        // if (bytes_sent == SOCKET_ERROR) {
        //     std::cerr << "Failed to send the data!";
        //     closesocket(WS::client_socket);
        //     WSACleanup();
        //     exit(-1);
        // }

        // closesocket(WS::client_socket);
        // WSACleanup();

        std::thread send_thread(SendInformation,std::ref(WS::client_socket));

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
