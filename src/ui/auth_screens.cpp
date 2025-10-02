#include "ui/chat_ui.h"
#include "client_auth.h"
#include <thread>
#include "chat.h"

#ifdef _WIN32
#define CLEAR_COMMAND "cls" 
#include "windows_client.h"

#elif __APPLE__
#include "unix_client.h"
#define CLEAR_COMMAND "clear"
#endif

namespace UI {

ftxui::Component RenderLoginScreen(ChatUI& ui) {
    static std::string status_message = "";
    
    auto username_input = ftxui::Input(&ui.username, "Enter username");
    
    ftxui::InputOption password_option;
    password_option.password = true;
    auto password_input = ftxui::Input(&ui.password, "Enter password", password_option);
    
    auto submit_button = ftxui::Button("Login", [&ui] {
        if (ui.username.empty() || ui.password.empty()) {
            status_message = "❌ All fields are required!";
            return;
        }
        
        status_message = "⏳ Logging in...";
        ui.GetScreen().Post(ftxui::Event::Custom);
        
        bool success = loginUser(ui.username, ui.password);
        
        if (success) {
            status_message = "✓ Login successful! Connecting to server...";
            ui.is_logged_in = true;
            
            // Initialize socket connection
            #ifdef _WIN32
            WS::Initialize();
            WS::CreateSocket();
            WS::ConnectWithServer();
            WS::SendData("hand_shake", "hand_shake");
            #elif __APPLE__
            US::createSocket();
            US::connectToServer();
            US::sendData("hand_shake", "hand_shake");
            #endif
            
            // Start receive thread (send will be handled per message)
            std::thread receive_thread(CHAT::recieveData);
            receive_thread.detach(); // Detach so it runs independently
            
            ui.current_state = AppState::CHAT_LOBBY;
            ui.GetScreen().ExitLoopClosure()();
        } else {
            status_message = "❌ Login failed!";
        }
    });
    
    auto back_button = ftxui::Button("Back", [&ui] {
        ui.username.clear();
        ui.password.clear();
        status_message = "";
        ui.current_state = AppState::MENU;
        ui.GetScreen().ExitLoopClosure()();
    });
    
    auto input_container = ftxui::Container::Vertical({
        username_input,
        password_input,
        submit_button,
        back_button
    });
    
    auto renderer = ftxui::Renderer(input_container, [=, &ui] {
        return ftxui::vbox({
            ftxui::text("LOGIN") | ftxui::bold | ftxui::center,
            ftxui::separator(),
            ftxui::text(""),
            ftxui::hbox({ftxui::text("Username: "), username_input->Render()}),
            ftxui::hbox({ftxui::text("Password: "), password_input->Render()}),
            ftxui::text(""),
            ftxui::text(status_message) | ftxui::center | (status_message.find("✓") != std::string::npos ? ftxui::color(ftxui::Color::Green) : ftxui::color(ftxui::Color::Red)),
            ftxui::text(""),
            submit_button->Render() | ftxui::center,
            back_button->Render() | ftxui::center,
        }) | ftxui::border | ftxui::center;
    });
    
    return renderer;
}

ftxui::Component RenderRegisterScreen(ChatUI& ui) {
    static std::string status_message = "";
    
    auto username_input = ftxui::Input(&ui.username, "Enter username");
    
    ftxui::InputOption password_option;
    password_option.password = true;
    auto password_input = ftxui::Input(&ui.password, "Enter password", password_option);
    
    ftxui::InputOption secret_option;
    secret_option.password = true;
    auto secret_input = ftxui::Input(&ui.secret_key, "Enter secret key", secret_option);
    
    auto submit_button = ftxui::Button("Submit", [&ui] {
        if (ui.username.empty() || ui.password.empty() || ui.secret_key.empty()) {
            status_message = "❌ All fields are required!";
            return;
        }
        
        status_message = "⏳ Registering...";
        ui.GetScreen().Post(ftxui::Event::Custom);
        
        bool success = registerUser(ui.username, ui.password, ui.secret_key);
        
        if (success) {
            status_message = "✓ Registration successful!";
            ui.username.clear();
            ui.password.clear();
            ui.secret_key.clear();
        } else {
            status_message = "❌ Registration failed!";
        }
    });
    
    auto back_button = ftxui::Button("Back", [&ui] {
        ui.username.clear();
        ui.password.clear();
        ui.secret_key.clear();
        status_message = "";
        ui.current_state = AppState::MENU;
        ui.GetScreen().ExitLoopClosure()();
    });
    
    auto input_container = ftxui::Container::Vertical({
        username_input,
        password_input,
        secret_input,
        submit_button,
        back_button
    });
    
    auto renderer = ftxui::Renderer(input_container, [=, &ui] {
        return ftxui::vbox({
            ftxui::text("REGISTER") | ftxui::bold | ftxui::center,
            ftxui::separator(),
            ftxui::text(""),
            ftxui::hbox({ftxui::text("Username  : "), username_input->Render()}),
            ftxui::hbox({ftxui::text("Password  : "), password_input->Render()}),
            ftxui::hbox({ftxui::text("Secret Key: "), secret_input->Render()}),
            ftxui::text(""),
            ftxui::text(status_message) | ftxui::center | (status_message.find("✓") != std::string::npos ? ftxui::color(ftxui::Color::Green) : ftxui::color(ftxui::Color::Red)),
            ftxui::text(""),
            submit_button->Render() | ftxui::center,
            back_button->Render() | ftxui::center,
        }) | ftxui::border | ftxui::center;
    });
    
    return renderer;
}

} // namespace UI
