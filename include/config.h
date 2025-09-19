#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace CONFIG {
    extern std::string client_username;
    extern const std::string SERVER_IP;
    extern const unsigned short SERVER_PORT_API;
    extern const unsigned short SERVER_PORT_SOCKET;
}
