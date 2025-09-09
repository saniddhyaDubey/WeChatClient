#include <iostream>
#include <thread>
#include <sstream>
#include <string>
#include "client_auth.h"

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#include "windows_client.h"

#elif __APPLE__
#include "unix_client.h"
#define CLEAR_COMMAND "clear"
#endif
bool testCPR();
static std::string rule_book = "Instructions: /register : Register  /login : Login  /switch username : switch chat   /quit : Exit\n";
int main()
{
    std::cout << "-------------------------------------------------WELCOME TO WECHAT-----------------------------------------\n";

    std::string user_input,username,password,secret_key;

    while (true)
    {
        std::cout<<rule_book<<std::endl;
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
        else if (command == "/login")
        {
            
        std::cout<<"Enter Username\n";
        std::cin>>username;
        std::cout<<"Enter Password\n";
        std::cin>>password;    
        std::cout<<"Enter secret key\n";
        std::cin>>secret_key;
        bool res = LoginUser(username,password,secret_key);
        


        #ifdef _WIN32

        #elif __APPLE__
            std::cout << "UNIX OS\n";
            US::createSocket(AF_INET, SOCK_STREAM, 0);
            US::connectToServer();

            std::string data_to_send = "Hey server, this is apple client!";

            int send_response = US::sendData(data_to_send);

            if (send_response == -1)
            {
                std::cerr << "Failed to send the data!";
                close(US::client_socket);
                // do we need to terminate the program or give any other option ?
            }
        #endif
        }
        else if (command == "/quit")
        {
            break;
        }
        else
        {
            std::cout << "Unknown command! Please check the instructions above" << std::endl;
        }
    }

    return 0;
}
