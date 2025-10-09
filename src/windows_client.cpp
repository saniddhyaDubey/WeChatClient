#ifdef _WIN32
#include "windows_client.h"

SOCKET WS::client_socket                = INVALID_SOCKET;

void WS::Initialize(){

    try
    {
        WSADATA wsadata;

        if(WSAStartup(MAKEWORD(2, 2), &wsadata) !=0){
            std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
            exit(-1);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        exit(-1);
    }
    
}


void WS::CreateSocket(){
    try{
        client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }catch(const std::exception& e){
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        exit(-1);
    }
}

void WS::ConnectWithServer(){
    try
    {
        SOCKADDR_IN server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(CONFIG::SERVER_PORT_SOCKET);

        if (InetPtonA(AF_INET, CONFIG::SERVER_IP.c_str(), &server_address.sin_addr) != 1) {
            std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
            closesocket(client_socket);
            WSACleanup();
            exit(-1);
        }

        if (connect(client_socket, reinterpret_cast<SOCKADDR*>(&server_address), sizeof(sockaddr)) != 0) {
            std::cerr << "\033[31m[UPDATE YOUR APPLICATION!!]\033[0m\n";
            closesocket(client_socket);
            WSACleanup();
            exit(-1);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        exit(-1);
    }
    
}


int WS::SendData(std::string user,std::string data){

    try
    {
        nlohmann::json data_packet = {
            {"sender" , CONFIG::client_username},
            {"receiver" , user},
            {"data" , data}
        };

        return send(client_socket, data_packet.dump().c_str(), data_packet.dump().length(), 0);
    }
    catch(const std::exception& e)
    {
         std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
         closesocket(client_socket);
         WSACleanup();
         exit(-1);
    }
    
}

std::pair<std::string,std::string> WS::RecieveData(){
    try
    {
        char data_load[1024];

        int bytes_recieved = recv(WS::client_socket,data_load,sizeof(data_load),0);

        if(bytes_recieved==SOCKET_ERROR || bytes_recieved==0){
            return {"Server","Close"};
        }
            
        std::string message_recieved = std::string(data_load,bytes_recieved);

        auto j_data = nlohmann::json::parse(message_recieved);

        std::string send_user = j_data.at("sender").get<std::string>();
        std::string data_to_send = j_data.at("data").get<std::string>();

        return {send_user,data_to_send};
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return {"Server","Close"};
    }
    
}
#endif
