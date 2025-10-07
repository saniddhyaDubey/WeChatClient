#ifdef __APPLE__
#include "unix_chat.h"
#include "unix_client.h"

namespace CD {
    struct termios old_tio, new_tio;
    std::string user_msg_buffer;
    std::string user_cmd_buffer;
    static bool is_chatting = false;
    std::mutex console_mutex;
    std::string rule_book = "\033[35mInstructions: /chat username : chat username   /logout : Logout   /stop : stop chatting with current user\033[0m\n";

    void init() {
        tcgetattr(STDIN_FILENO, &old_tio);
        
        new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    }

    void cleanUpTio() {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }

    void sendData(){
        init();
        std::cout<<rule_book;
        std::cout.flush();

        while(true){

            while(true){
                char cmd_ch;
                read(STDIN_FILENO, &cmd_ch, 1);

                if(cmd_ch == '\n' || cmd_ch == '\r'){
                    std::cout << "\n";
                    break;
                }else if(cmd_ch == 127 || cmd_ch == 8){
                    if(!user_cmd_buffer.empty()) {
                        user_cmd_buffer.pop_back();
                        std::cout << "\b \b";
                        std::cout.flush();
                    }
                }else if(cmd_ch >= 32 && cmd_ch < 127) {
                    user_cmd_buffer += cmd_ch;
                    std::cout << cmd_ch;
                    std::cout.flush();
                }
            }

            std::string command_switch, command_user;
            // std::getline(std::cin, user_input);
            std::istringstream iss(user_cmd_buffer);
            iss >> command_switch;
            iss >> command_user;

            if(command_switch=="/logout"){
                user_cmd_buffer.clear();
                cleanUpTio();
                break;
            }else if(command_switch=="/chat" && command_user!=""){
                if(command_user==CONFIG::client_username){
                    std::cout<<"Change username!\n";
                    continue;
                }

                user_cmd_buffer.clear();
                is_chatting = true;
                
                while(true){
                    std::cout<<"\033[32m[YOU]:\033[0m ";
                    std::cout.flush();
                    while(true){
                        char ch;
                        read(STDIN_FILENO, &ch, 1);

                        if(ch == '\n' || ch == '\r') {
                            std::cout << "\n";
                            break;
                        }else if(ch == 127 || ch == 8) {
                            if(!user_msg_buffer.empty()) {
                                user_msg_buffer.pop_back();
                                std::cout << "\b \b";
                                std::cout.flush();
                            }
                        }else if(ch >= 32 && ch < 127) {
                            user_msg_buffer += ch;
                            std::cout << ch;
                            std::cout.flush();
                        }
                    }
                    
                    if(user_msg_buffer=="/stop"){
                        user_msg_buffer.clear();
                        is_chatting = false;

                        break;
                    }
                    
                    if(user_msg_buffer.empty() || user_msg_buffer == " "){
                        std::cout<<"Blank Message\n";
                        user_msg_buffer.clear();
                        continue;
                    }
                    int send_response = US::sendData(user_msg_buffer, command_user);
                    user_msg_buffer.clear();
                }
            }else {
                std::cout<<"Incorrect command! Please follow Instructions listed above.\n";
            }
        }
    }

    void receiveData(){
        while(true){
            std::pair<std::string, std::string> received_messages = US::receiveData();
            if(received_messages.first=="Server" && received_messages.second=="Close") {
                break;
            }
            {
                std::lock_guard<std::mutex> lock(console_mutex);
                
                // Clear the current input line
                std::cout << "\r\033[K";

                if(is_chatting){
                    // Print: [SENDER]: message
                    std::cout << "\033[36m[" << received_messages.first << "]:\033[0m " << received_messages.second << "\n";
                    
                    // Redraw the input prompt with current buffer
                    std::cout << "\033[32m[YOU]:\033[0m " << user_msg_buffer;
                    std::cout.flush();
                }else{
                    // Print: [SENDER]: message
                    std::cout << "\033[31m[New message from " << received_messages.first << ": " << received_messages.second << "]\033[0m\n";
                    
                    // Redraw the input prompt with current buffer
                    std::cout << user_cmd_buffer;
                    std::cout.flush();
                }                
            }
        }
    } 

}
#endif
