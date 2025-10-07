#ifdef __APPLE__
#include "unix_chat.h"
#include "unix_client.h"

namespace CD {
    struct termios old_tio, new_tio;
    std::string user_msg_buffer;
    std::string user_cmd_buffer;
    static bool is_chatting = false;
    std::mutex console_mutex;
    std::string current_recipient;
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
        try
        {
            init();
            std::cout<<rule_book;
            std::cout.flush();

            while(true){

                std::cout<< "> : ";
                std::cout.flush();

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

                std::string command_switch;
                // std::getline(std::cin, user_input);
                std::istringstream iss(user_cmd_buffer);
                iss >> command_switch;
                iss >> current_recipient;

                if(command_switch=="/logout"){
                    user_cmd_buffer.clear();
                    cleanUpTio();
                    break;
                }else if(command_switch=="/chat" && current_recipient!=""){
                    if(current_recipient==CONFIG::client_username){
                        std::cout<<"\n\033[31mEcho! Change username.\033[0m\n> : ";
                        continue;
                    }

                    user_cmd_buffer.clear();
                    is_chatting = true;
                    std::string is_blank_message;
                    
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

                        std::istringstream issb(user_msg_buffer);
                        is_blank_message.clear();
                        issb>>is_blank_message;

                        if(is_blank_message.empty()){
                            std::cout<<"\033[31mBlank Message\033[0m\n";
                            user_msg_buffer.clear();
                            continue;
                        }
                        
                        if(user_msg_buffer=="/stop"){
                            user_msg_buffer.clear();
                            is_chatting = false;
                            break;
                        }
                        
                        int send_response = US::sendData(user_msg_buffer, current_recipient);
                        user_msg_buffer.clear();
                    }
                }else {
                    std::cout << "\033[31mIncorrect command! Please follow Instructions listed above.\033[0m\n";
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
            exit(-1);
        }
        
    }

    void receiveData(){
        try
        {
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

                        if(received_messages.first != current_recipient){
                            // Print: [SENDER]: message
                            std::cout << "\033[31m[New message from " << received_messages.first << ": " << received_messages.second << "]\033[0m\n";
                        }else{
                            // Print: [SENDER]: message
                            std::cout << "\033[36m[" << received_messages.first << "]:\033[0m " << received_messages.second << "\n";
                        }
                        
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
        catch(const std::exception& e)
        {
            std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
            exit(-1);
        }
        
    } 

}
#endif
