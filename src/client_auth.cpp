#include "client_auth.h"

bool registerUser(const std::string &username, const std::string &password, const std::string &secret_key){
    try
    {
        if(username.empty() || password.empty() || secret_key.empty()){
            std::cerr << "Error: Enter all the three details\n";
            return false;
        }

        const std::string SERVER_URL = std::format("http://{}:{}/register", CONFIG::SERVER_IP, CONFIG::SERVER_PORT_API);
    
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
            std::cerr << "\033[92m[Successfull Registration!]\033[0m\n";
            return true;   
        }

        else if(register_response.status_code == (long) 403){
            std::cerr << "\033[31m[Wrong Secret Key!]\033[0m\n";
            return false;   
        }

        else if(register_response.status_code == (long) 409){
            std::cerr << "\033[31m[Username taken!]\033[0m\n";
            return false;   
        }

        else if(register_response.status_code == (long) 500){
            std::cerr << "\033[31m[SERVER IS SICK!!]\033[0m\n";
            return false;   
        }

        return false;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return false;
    }
    
    
}

bool loginUser(std::string username, std::string password){
    try
    {
        if(username.empty() || password.empty()){
            std::cerr << "Error: Enter all details\n";
            return false;
        }

        nlohmann::json json_body = {
            {"username" , username},
            {"password" , password}
        };

        std::string url = std::format("http://{}:{}/login", CONFIG::SERVER_IP, CONFIG::SERVER_PORT_API);
        cpr::Response login_response = cpr::Post(
        cpr::Url{url},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{json_body.dump()}
        );

        if (login_response.status_code == (long) 200) {
            std::cout << "\033[92m[Login Success!]\033[0m\n";
            CONFIG::client_username = username;
            return true;
        }
        else if(login_response.status_code == (long) 400){
            std::cerr << "\033[31m[Credentials Not Found!]\033[0m\n";
            return false;   
        }
        else if(login_response.status_code == (long) 401){
            std::cerr << "\033[31mWrong Credentials]\033[0m\n";
            return false;   
        }
        else if(login_response.status_code == (long) 500){
            std::cerr << "\033[31m[Login failed!]\033[0m\n";
            return false;   
        }
        else if(login_response.status_code == (long) 250){
            std::cerr << "\033[31m[Duplicate session!]\033[0m\n";
            return false;   
        }
        else{
            std::cerr << "\033[31m[SERVER IS SICK!!]\033[0m\n";
        }

        return false;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return false;
    }
}
