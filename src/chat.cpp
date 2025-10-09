#ifdef _WIN32
#include "chat.h"
#include "config.h"
#include "windows_client.h"
#include <conio.h>
#include <mutex>
#include <string>
#include <iostream>
static bool is_chatting = false;
static std::mutex consoleMutex;  
static std::string inputBuffer,commandBuffer,other_user;


static std::string rule_book =
    "\033[35mInstructions: /chat username : chat username   \033[0m"
    "\033[35m/logout : Logout   /stop : stop chatting with current user\033[0m\n";


void CHAT::sendData(){
    try
    {
        std::cout << rule_book;
        std::cout<<"> : ";
        while(true){
            char cmd = _getch();

            if(cmd == '\r'){

                std::istringstream iss(commandBuffer);

                inputBuffer.clear();
                commandBuffer.clear();
                std::string chatcommand, command_user, extra;  
                iss >> chatcommand >> command_user >> extra; 

                if (!chatcommand.empty() && !command_user.empty() && extra.empty() && chatcommand=="/chat") {
                    if(command_user==CONFIG::client_username){
                        std::cout<<"\n\033[31mEcho! Change username.\033[0m\n> : ";
                        continue;
                    }
                    std::string is_blank_message;
                    is_chatting = true;
                    other_user = command_user;
                    std::cout << "\n\033[32mYou : \033[0m";
                    while(true){
                        char ch = _getch();

                        if(ch == '\r'){ 
                            std::istringstream issb(inputBuffer);
                            is_blank_message.clear();
                            issb>>is_blank_message;
                            if(inputBuffer=="/stop"){
                                inputBuffer.clear();
                                is_chatting = false;
                                std::cout<<"\n> : ";
                                break;
                            }
                            if(is_blank_message.empty()) {
                                std::cout << "\033[31mBlank Message\033[0m\n\033[32mYou : \033[0m";
                                inputBuffer.clear();
                                continue;
                            }

                            WS::SendData(command_user, inputBuffer);

                            std::cout << "\n\033[32mYou : \033[0m";
                            inputBuffer.clear();
                        }
                        else if(ch == '\b'){ 
                            if(!inputBuffer.empty()){
                                inputBuffer.pop_back();
                                std::cout << "\b \b" << std::flush;
                            }
                        }
                        else if(ch >= 32 && ch <= 126){ 
                            inputBuffer.push_back(ch);
                            std::cout << ch << std::flush;
                        }
                    }
                } 
                else if(!chatcommand.empty() && chatcommand=="/logout" && command_user.empty() && extra.empty()){
                    std::cout<<'\n';
                    break;
                }
                else {
                    std::cout << "\n\033[31mIncorrect command! Please follow Instructions listed above.\033[0m\n> : ";
                }

            }
            else if(cmd == '\b'){
                if(!commandBuffer.empty()){
                    commandBuffer.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
            }
            else if(cmd >= 32 && cmd <= 126){ 
                commandBuffer.push_back(cmd);
                std::cout << cmd << std::flush;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return;
    }
    
}


void CHAT::recieveData(){
    try
    {
        while(true){
            std::pair<std::string,std::string> message_recieved = WS::RecieveData();
            if(message_recieved.first=="Server" && message_recieved.second=="Close") break;
            {
                std::lock_guard<std::mutex> lock(consoleMutex);
                std::cout << "\r\033[K\r";
                if(is_chatting){

                    if(other_user!=message_recieved.first){
                        std::cout <<"\033[31m[New message from "<<message_recieved.first <<" : "<< message_recieved.second<< "]\n\033[32mYou : \033[0m" << inputBuffer<<std::flush;
                    }
                    else{
                        std::cout <<"\033[36m"<<message_recieved.first <<" : \033[0m"<< message_recieved.second<< "\n\033[32mYou : \033[0m" << inputBuffer<<std::flush;
                    }
                }
                else{
                    std::cout <<"\033[31m[New message from "<<message_recieved.first<<" : "<<message_recieved.second << "]\033[0m\n> : " << commandBuffer<<std::flush;
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return;
    }
    
}

#endif