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

    if(register_response.status_code == (long) 200){
        std::cerr << "Successfull Registration! You can login now.\n";
        return true;   
    }

    else if(register_response.status_code == (long) 403){
        std::cerr << "Incorrect Secret Key!\n";
        return false;   
    }

    else if(register_response.status_code == (long) 409){
        std::cerr << "Username taken! Please use another username.\n";
        return false;   
    }

    else if(register_response.status_code == (long) 500){
        std::cerr << "Registration failed! Server error.\n";
        return false;   
    }

    return true;
}

bool loginUser(std::string username, std::string password){

    nlohmann::json json_body = {
        {"username" , username},
        {"password" , password}
    };

    std::string url = std::format("http://{}:{}/login", CONFIG::SERVER_IP, CONFIG::SERVER_PORT);
    std::cout<<url<<std::endl;
    cpr::Response r = cpr::Post(
    cpr::Url{url},
    cpr::Header{{"Content-Type", "application/json"}},
    cpr::Body{json_body.dump()}
    );

    if (r.status_code == (long) 200) {
        std::cout << "Login successful!\nResponse:" << r.text << std::endl;
        return true;
    }

    else if(register_response.status_code == (long) 400){
        std::cerr << "Please enter username and password!\n";
        return false;   
    }

    else if(register_response.status_code == (long) 401){
        std::cerr << "Invalid Username or password.\n";
        return false;   
    }

    else if(register_response.status_code == (long) 500){
        std::cerr << "Login failed! Server error.\n";
        return false;   
    }
    
    return true;
}
