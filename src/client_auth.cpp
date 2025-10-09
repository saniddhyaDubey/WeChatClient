#include "client_auth.h"

void registerUser(const std::string &input_username, const std::string &input_password, const std::string &sinput_secret_key){
    try
    {
        std::string username,password,secret_key,username_with_gap,password_with_gap;
        std::istringstream iss;
        iss.str(input_username);
        iss>>username>>username_with_gap;
        if(!username_with_gap.empty()){
            std::cerr << "\033[31m[Username must not contain spaces.!]\033[0m\n";
            return;
        }
        iss.clear();
        iss.str("");
        iss.str(input_password);
        iss>>password>>password_with_gap;
        if(!password_with_gap.empty()){
            std::cerr << "\033[31m[Password must not contain spaces.!]\033[0m\n";
            return;
        }
        iss.clear();
        iss.str("");
        iss.str(sinput_secret_key);
        iss>>secret_key;

        if(username.empty() || password.empty() || secret_key.empty()){
            std::cerr << "\033[31m[Enter all details!]\033[0m\n";
            return;
        }
        
        if(username.length()>20 || password.length()>20){
            std::cerr << "\033[31m[Username and password must be less than or 20 characters!]\033[0m\n";
            return;
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
            return;
        }

        else if(register_response.status_code == (long) 403){
            std::cerr << "\033[31m[Wrong Secret Key!]\033[0m\n";
            return;
        }

        else if(register_response.status_code == (long) 409){
            std::cerr << "\033[31m[Username taken!]\033[0m\n";
            return;
        }
        else{
            std::cerr << "\033[31m[CANNOT REGISTER..SERVER IS SICK!!]\033[0m\n";
            return;
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return;
    }
    
    
}

bool loginUser(std::string input_username, std::string input_password){
    try
    {
        std::string username,password,username_with_gap,password_with_gap;
        std::istringstream iss;
        iss.str(input_username);
        iss>>username>>username_with_gap;
        if(!username_with_gap.empty()){
            std::cerr << "\033[31m[Wrong Credentials]\033[0m\n";
            return false;
        }
        iss.clear();
        iss.str("");
        iss.str(input_password);
        iss>>password>>password_with_gap;
        if(!password_with_gap.empty()){
            std::cerr << "\033[31m[Wrong Credentials]\033[0m\n";
            return false;
        }
        if(username.empty() || password.empty()){
            std::cerr << "\033[31m[Enter all details!]\033[0m\n";
            return false;
        }

        if(username.length()>20 || password.length()>20){
            std::cerr << "\033[31m[Wrong Credentials]\033[0m\n";
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
            std::cerr << "\033[31m[Wrong Credentials]\033[0m\n";
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
            std::cerr << "\033[31m[CANNOT LOGIN..SERVER IS SICK!!]\033[0m\n";
        }

        return false;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m[SOMETHING WENT WRONG!!]\033[0m\n";
        return false;
    }
}
