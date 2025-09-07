#include <iostream>
#include <thread>
#include <sstream>
#include <string>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#include "windows_client.h"

void SendInformation(SOCKET &client_socket)
{
    std::string user_input;
}

#elif __APPLE__
#include "unix_client.h"
#define CLEAR_COMMAND "clear"
#endif

static std::string rule_book = "Instructions: /register : Register  /login : Login  /switch username : switch chat   /quit : Exit\n";

int main()
{
    std::cout << "-------------------------------------------------WELCOME TO WECHAT-----------------------------------------\n";

    std::string user_input;
    while (true)
    {
        std::cout<<rule_book<<std::endl;
        std::getline(std::cin, user_input);
        std::istringstream iss(user_input);
        std::string command;
        iss >> command;

        if (command == "/register")
        {
        }
        else if (command == "/login")
        {
            // handle login

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
        else if (command == "quit")
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
