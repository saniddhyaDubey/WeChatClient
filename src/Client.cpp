#include <iostream>
#include <thread>
#include <sstream>
#include <string>
#include "client_auth.h"
#ifdef _WIN32
#define CLEAR_COMMAND "cls" 
#include "windows_client.h"
#include "chat.h"
#elif __APPLE__
#include "unix_client.h"
#include "unix_chat.h"
#define CLEAR_COMMAND "clear"
#endif


static std::string rule_book = "\033[35mInstructions: /register : Register  /login : Login  /switch username : switch chat   /quit : Exit\033[0m\n> : ";

int main()
{
    std::cout << "-------------------------------------------------WELCOME TO WECHAT-------------------------------------------------\n";

    std::string user_input,username,password,secret_key;

    while (true)
    {
        std::cout<<rule_book;
        std::getline(std::cin, user_input);
        std::istringstream iss(user_input);
        std::string command;
        iss >> command;

        if (command == "/register")
        {
            std::cout << "Enter username: ";
            std::getline(std::cin, username);

            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            std::cout << "Enter secret key: ";
            std::getline(std::cin, secret_key);

            bool user_register = registerUser(username, password, secret_key);
        }
        else if (command == "/login"){
            
        std::cout<<"Enter Username: ";
        std::getline(std::cin, username);
        std::cout<<"Enter Password: ";
        std::getline(std::cin, password);
        bool login_result = loginUser(username,password);

        #ifdef _WIN32

        if(login_result){
            WS::Initialize();
            WS::CreateSocket();
            WS::ConnectWithServer();
            WS::SendData("hand_shake","hand_shake");
            std::thread send_data_thread(CHAT::sendData);
            std::thread recieve_data_thread(CHAT::recieveData);
            send_data_thread.join();
            closesocket(WS::client_socket);
            WSACleanup();
            recieve_data_thread.join();
        }

        #elif __APPLE__

        if(login_result){
            US::createSocket();
            US::connectToServer();
            US::sendData("hand_shake", "hand_shake");
            // CD::init();
            std::thread send_data_thread(CD::sendData);
            std::thread recieve_data_thread(CD::receiveData);
            send_data_thread.join();
            close(US::client_socket);
            // CD::cleanUpTio();
            recieve_data_thread.join();
        }

        #endif
        }else if (command == "/quit"){
            break;
        }else{
            std::cout << "Unknown command! Please check the instructions above" << std::endl;
        }
    }
    return 0;
}
