#include "chat.h"
#include "config.h"
static std::string rule_book = "Instructions: /chat username : chat username   /logout : Logout   /stop : stop chatting with current user\n";


void CHAT::sendData(){
    std::cout<<rule_book;  

    while(true){

        std::string user_input,command_switch,command_user,data_to_send;
        std::getline(std::cin, user_input);
        std::istringstream iss(user_input);
        iss >> command_switch;
        iss >> command_user;

        if(command_switch=="/logout"){
            break;
        }

        else if(command_switch=="/chat" && command_user!=""){
            if(command_user==CONFIG::client_username){
                std::cout<<"Echo! Change username.\n";
                continue;
            }
            while(true){
                std::cout<<"Enter the message : ";

                getline(std::cin,data_to_send);

                if(data_to_send=="/stop") break;

                if(data_to_send=="" || data_to_send==" "){
                    std::cout<<"Blank Message\n";
                    continue;
                }

                #ifdef _WIN32

                int bytes_send  = WS::SendData(command_user,data_to_send);

                #elif __APPLE__

                //-------------------- apple------------------

                #endif
            }

        } 

        else {
            std::cout<<"Incorrect command! Please follow Instructions listed above.\n";
        }

    }

}

void CHAT::recieveData(){

    std::string extracted_message;
    while(true){

        #ifdef _WIN32

        std::string message_recieved = WS::RecieveData();

        if(message_recieved=="Socket closed!") break;

        std::cout<<message_recieved<<'\n';

        #elif __APPLE__

        // ---------------------apple----------------
        #endif
    }
}   