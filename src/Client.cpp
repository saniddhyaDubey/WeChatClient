#include <iostream>
#include "ui/chat_ui.h"

int main()
{
    std::cout << "Starting UI..." << std::endl;
    UI::ChatUI ui;
    std::cout << "UI created, calling Run()..." << std::endl;
    ui.Run();
    std::cout << "UI exited." << std::endl;
    return 0;
}


// #include <iostream>

// #include <sstream>
// #include <string>
// #include "chat.h"

// #include "ui/chat_ui.h"




// static std::string rule_book = "Instructions: /register : Register  /login : Login  /switch username : switch chat   /quit : Exit\n";

// int main()
// {
//     UI::ChatUI ui;
//     ui.Run();
//     return 0;

//     std::cout << "-------------------------------------------------WELCOME TO WECHAT-----------------------------------------\n";

//     std::string user_input,username,password,secret_key;

//     // bool login_result = loginUser("saniddhya", "Qwerty@1234!");

//     // return 0;

//     while (true)
//     {
//         std::cout<<rule_book<<std::endl;
//         std::getline(std::cin, user_input);
//         std::istringstream iss(user_input);
//         std::string command;
//         iss >> command;

//         if (command == "/register")
//         {
//             std::cout << "Enter username: ";
//             std::getline(std::cin, username);

//             std::cout << "Enter password: ";
//             std::getline(std::cin, password);

//             std::cout << "Enter secret key: ";
//             std::getline(std::cin, secret_key);

//             bool user_register = registerUser(username, password, secret_key);
//         }
//         else if (command == "/login"){
            
//         std::cout<<"Enter Username: ";
//         std::getline(std::cin, username);
//         std::cout<<"Enter Password: ";
//         std::getline(std::cin, password);
//         bool login_result = loginUser(username,password);

//         // return 0;

//         #ifdef _WIN32

//         if(login_result){
//             WS::Initialize();
//             WS::CreateSocket();
//             WS::ConnectWithServer();
//             WS::SendData("hand_shake","hand_shake");
//             std::thread send_data_thread(CHAT::sendData);
//             std::thread recieve_data_thread(CHAT::recieveData);
//             send_data_thread.join();
//             closesocket(WS::client_socket);
//             WSACleanup();
//             recieve_data_thread.join();
//         }

//         #elif __APPLE__

//         if(login_result){
//             US::createSocket();
//             US::connectToServer();
//             US::sendData("hand_shake", "hand_shake");
//             std::thread send_data_thread(CHAT::sendData);
//             std::thread recieve_data_thread(CHAT::recieveData);
//             send_data_thread.join();
//             close(US::client_socket);
//             recieve_data_thread.join();
//         }

//         #endif
//         }else if (command == "/quit"){
//             break;
//         }else{
//             std::cout << "Unknown command! Please check the instructions above" << std::endl;
//         }
//     }
//     return 0;
// }
