#ifdef _WIN32
#include "windows_client.h"

SOCKET WS::client_socket                = INVALID_SOCKET;

void WS::Initialize(){

    WSADATA wsadata;

    if(WSAStartup(MAKEWORD(2, 2), &wsadata) !=0){
        std::cerr << "Failed to create socket address!";
        exit(-1);
    }
}


void WS::CreateSocket(){

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

}

void WS::ConnectWithServer(){
    SOCKADDR_IN server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONFIG::SERVER_PORT_SOCKET);

    if (InetPtonA(AF_INET, CONFIG::SERVER_IP.c_str(), &server_address.sin_addr) != 1) {
        std::cerr << "Failed to create socket address!";
        closesocket(client_socket);
        WSACleanup();
        exit(-1);
    }

    if (connect(client_socket, reinterpret_cast<SOCKADDR*>(&server_address), sizeof(sockaddr)) != 0) {
        std::cerr << "Failed to connect with server!";
        closesocket(client_socket);
        WSACleanup();
        exit(-1);
    }
}


int WS::SendData(std::string user,std::string data){

    nlohmann::json data_packet = {
        {"sender" , CONFIG::client_username},
        {"receiver" , user},
        {"data" , data}
    };

    return send(client_socket, data_packet.dump().c_str(), data_packet.dump().length(), 0);
}

std::string WS::RecieveData(){
    char data_load[1024];

    int bytes_recieved = recv(WS::client_socket,data_load,sizeof(data_load),0);

    if(bytes_recieved==SOCKET_ERROR){
        return "Socket closed!";
    }
        
    std::string message_recieved = std::string(data_load,bytes_recieved);

    auto j_data = nlohmann::json::parse(message_recieved);

    std::string send_user = j_data.at("sender").get<std::string>();
    std::string data_to_send = j_data.at("data").get<std::string>();

    return send_user + " : " + data_to_send;
}
#endif
