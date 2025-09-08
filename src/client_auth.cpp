// Register and login function
#include <iostream>
#include <format>
#include <string>
#include "config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

bool registerUser(const std::string &username, const std::string &password, const std::string &secret_key){
    if(username.empty() || password.empty() || secret_key.empty()){
        std::cerr << "Error: Enter all the three details\n";
        return false;
    }

    const std::string SERVER_URL = std::format("http://{}:{}/register", CONFIG::SERVER_IP, CONFIG::SERVER_PORT);

    nlohmann::json request_body_json = {
        {"username", username},
        {"password", password},
        {"secretKey", secret_key}
    };

    const std::string request_body_string = request_body_json.dump();

    cpr::Response register_response = cpr::Post(
        cpr::Url{SERVER_URL},
        cpr::Body{request_body_string},
        cpr::Header{{"accept", "application/json"}}
    );

    if(register_response.status_code != (long) 200){
        std::cerr << "Error: Registration Failed\n";
        return false;   
    }

    std::cout << "User registered successfully!\n";
    return true;
}
