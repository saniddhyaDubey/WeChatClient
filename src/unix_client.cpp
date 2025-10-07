#ifdef __APPLE__
#include "config.h"
#include "unix_client.h"

int US::client_socket;

void US::createSocket(){
    try
    {
        client_socket = socket(AF_INET, SOCK_STREAM, 0);

        if(client_socket < 0){
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

void US::connectToServer(){
    try
    {
        sockaddr_in server_info;
        server_info.sin_family = AF_INET;
        server_info.sin_port = htons(CONFIG::SERVER_PORT_SOCKET);

        if(inet_pton(AF_INET, CONFIG::SERVER_IP.c_str(), &server_info.sin_addr.s_addr) != 1){   //Error case
            std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
            close(client_socket);
            return;
        }

        // std::cout<<"Server configured! \n";

        int connection_response = connect(client_socket, (struct sockaddr*)&server_info, sizeof(server_info));

        // std::cout<<"connect called! \n";

        if(connection_response != 0){
            std::cerr << "\033[31m[UPDATE YOUR APPLICATION!!]\033[0m\n";
            close(client_socket);
            return;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        exit(-1);
    }
    
}

int US::sendData(std::string data, std::string recipient_user){
    
    try
    {
        nlohmann::json data_packet = {
            {"sender" , CONFIG::client_username},
            {"receiver" , recipient_user},
            {"data" , data}
        };

        return send(client_socket, data_packet.dump().c_str(), data_packet.dump().length(), 0);
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        exit(-1);
    }
    
}

std::pair<std::string, std::string> US::receiveData(){
    char buffer[1024];
    ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if(bytes_read <= 0) {
        return {"Server", "Close"};
    }

    try {
        buffer[bytes_read] = '\0';
        nlohmann::json data_packet = nlohmann::json::parse(buffer);

        std::string send_user = data_packet.at("sender").get<std::string>();
        std::string data_to_send = data_packet.at("data").get<std::string>();

        return {send_user, data_to_send};

    } catch(const nlohmann::json::exception& e) {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        exit(-1);
    }
}

#endif
