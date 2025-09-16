#include "client_auth.h"

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

bool loginUser(std::string username, std::string password, std::string secret_key){

    nlohmann::json json_body = {
        {"username" , username},
        {"password" , password},
        {"secret_key" , secret_key}
    };

    std::string url = std::format("http://{}:{}/login", CONFIG::SERVER_IP, CONFIG::SERVER_PORT);
    std::cout<<url<<std::endl;
    cpr::Response r = cpr::Post(
    cpr::Url{url},
    cpr::Header{{"Content-Type", "application/json"}},
    cpr::Body{json_body.dump()}
    );

    std::cout<<r.status_code<<std::endl;

    if (r.status_code == 200) {
    std::cout << "Login successful! Response:\n" << r.text << std::endl;
    return true;
    } else {
        std::cerr << "Login failed. Status code: " << r.status_code << std::endl;
    }
    return false;
}
