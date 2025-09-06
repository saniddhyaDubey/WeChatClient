#include "unix_client.h"

namespace US {
    int client_socket;
    sockaddr_in server_info;

    void createSocket(int domain, int type, int protocol){
        client_socket = socket(domain, type, protocol);

        if(client_socket < 0){
            std::cerr << "Error: Creating socket \n";
            return;
        }

        std::cout<<"Client Socket is created! \n";
    }

    void connectToServer(){
        std::cout<<"Connect to server function called! \n";
        const int SERVER_PORT = 12345;
        const std::string SERVER_IP_ADDRESS = "10.106.1.36";

        server_info.sin_family = AF_INET;
        server_info.sin_port = htons(SERVER_PORT);

        if(inet_pton(AF_INET, SERVER_IP_ADDRESS.c_str(), &server_info.sin_addr.s_addr) != 1){   //Error case
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

        const char* message = "Hello, server!";
        send(client_socket, message, strlen(message), 0);

        close(client_socket);
    }
};
