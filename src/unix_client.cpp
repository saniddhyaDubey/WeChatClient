#ifdef __APPLE__
#include "config.h"
#include "unix_client.h"

int US::client_socket;
sockaddr_in US::server_info;

void US::createSocket(int domain, int type, int protocol){
    client_socket = socket(domain, type, protocol);

    if(client_socket < 0){
        std::cerr << "Error: Creating socket \n";
        exit(-1);
    }

    std::cout<<"Client Socket is created! \n";
}

void US::connectToServer(){
    std::cout<<"Connect to server function called! \n";

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(CONFIG::SERVER_PORT);

    if(inet_pton(AF_INET, CONFIG::SERVER_IP.c_str(), &server_info.sin_addr.s_addr) != 1){   //Error case
        std::cerr << "Error: Server configuration \n";
        close(client_socket);
        return;
    }

    std::cout<<"Server configured! \n";

    int connection_response = connect(client_socket, (struct sockaddr*)&server_info, sizeof(server_info));

    std::cout<<"connect called! \n";

    if(connection_response != 0){
        std::cerr << "Error: Connection issue. Err No: "<< strerror(errno) << '\n';
        close(client_socket);
        return;
    }

    std::cout<<"Connected! \n";
}

int US::sendData(std::string data){
    return send(client_socket, message.c_str(), strlen(message), 0);
}
#endif
