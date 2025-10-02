#include "ui/chat_ui.h"
#ifdef _WIN32
#define CLEAR_COMMAND "cls" 
#include "windows_client.h"

#elif __APPLE__
#include "unix_client.h"
#define CLEAR_COMMAND "clear"
#endif

namespace UI {

    // Thread-safe message storage
    struct Message {
        std::string sender;
        std::string content;
        bool is_outgoing;
    };

    static std::vector<Message> messages;
    static std::mutex msg_mutex;
    static ftxui::ScreenInteractive* active_screen = nullptr;

ftxui::Component RenderChatLobbyScreen(ChatUI& ui) {
    static std::string command_input = "";
    static std::string status_message = "";
    
    auto input_field = ftxui::Input(&command_input, "Enter command");
    
    auto submit_button = ftxui::Button("Submit", [&ui] {
        if (command_input.empty()) {
            status_message = "❌ Please enter a command!";
            return;
        }
        
        // Parse command
        if (command_input.find("/chat ") == 0) {
            std::string recipient = command_input.substr(6); // Get username after "/chat "
            if (recipient.empty()) {
                status_message = "❌ Please specify a username!";
                return;
            }
            ui.current_recipient = recipient;
            ui.current_state = AppState::CHATTING;
            command_input.clear();
            status_message = "";
            ui.GetScreen().ExitLoopClosure()();
        } 
        else if (command_input == "/logout") {
            ui.is_logged_in = false;
            ui.username.clear();
            ui.password.clear();
            ui.current_state = AppState::MENU;
            command_input.clear();
            status_message = "";
            ui.GetScreen().ExitLoopClosure()();
        }
        else {
            status_message = "❌ Unknown command! Use /chat username or /logout";
        }
    });
    
    auto container = ftxui::Container::Vertical({
        input_field,
        submit_button
    });
    
    auto renderer = ftxui::Renderer(container, [=, &ui] {
        return ftxui::vbox({
            ftxui::text("CHAT LOBBY") | ftxui::bold | ftxui::center,
            ftxui::text("Logged in as: " + ui.username) | ftxui::center,
            ftxui::separator(),
            ftxui::text(""),
            ftxui::text("Instructions:") | ftxui::bold,
            ftxui::text("  /chat username : Start chat with username"),
            ftxui::text("  /logout        : Logout"),
            ftxui::text(""),
            ftxui::separator(),
            ftxui::text(""),
            ftxui::hbox({ftxui::text("Command: "), input_field->Render()}),
            ftxui::text(""),
            ftxui::text(status_message) | ftxui::center | ftxui::color(ftxui::Color::Red),
            ftxui::text(""),
            submit_button->Render() | ftxui::center,
        }) | ftxui::border | ftxui::center;
    });
    
    return renderer;
}

ftxui::Component RenderChattingScreen(ChatUI& ui) {
    static std::string input_message = "";
    
    auto input_field = ftxui::Input(&input_message, "Type your message");
    
    auto send_button = ftxui::Button("Send", [&ui] {
        if (input_message.empty()) return;
        
        // Add to local messages
        {
            std::lock_guard<std::mutex> lock(msg_mutex);
            messages.push_back({ui.username, input_message, true});
        }
        
        // TODO: Send to server via socket
        #ifdef _WIN32
        WS::SendData(ui.current_recipient, input_message);
        #elif __APPLE__
        US::sendData(input_message, ui.current_recipient);
        #endif
        
        input_message.clear();
        ui.GetScreen().Post(ftxui::Event::Custom);
    });
    
    auto stop_button = ftxui::Button("Stop Chat", [&ui] {
        messages.clear();
        ui.current_recipient.clear();
        ui.current_state = AppState::CHAT_LOBBY;
        input_message.clear();
        ui.GetScreen().ExitLoopClosure()();
    });
    
    auto container = ftxui::Container::Vertical({
        input_field,
        send_button,
        stop_button
    });
    
    auto renderer = ftxui::Renderer(container, [=, &ui] {
        // Build message list
        ftxui::Elements message_elements;
        {
            std::lock_guard<std::mutex> lock(msg_mutex);
            for (const auto& msg : messages) {
                if (msg.is_outgoing) {
                    message_elements.push_back(
                        ftxui::hbox({
                            ftxui::text("You: ") | ftxui::bold | ftxui::color(ftxui::Color::Green),
                            ftxui::text(msg.content)
                        })
                    );
                } else {
                    message_elements.push_back(
                        ftxui::hbox({
                            ftxui::text(msg.sender + ": ") | ftxui::bold | ftxui::color(ftxui::Color::Blue),
                            ftxui::text(msg.content)
                        })
                    );
                }
            }
        }
        
        return ftxui::vbox({
            // Header
            ftxui::hbox({
                ftxui::text("Chatting with: ") | ftxui::bold,
                ftxui::text(ui.current_recipient) | ftxui::bold | ftxui::color(ftxui::Color::Cyan)
            }),
            ftxui::separator(),
            
            // Message area (scrollable)
            ftxui::vbox(message_elements) | ftxui::flex | ftxui::yframe,
            
            ftxui::separator(),
            
            // Input area
            ftxui::hbox({ftxui::text("> "), input_field->Render()}),
            ftxui::text(""),
            ftxui::hbox({
                send_button->Render(),
                ftxui::text("  "),
                stop_button->Render()
            }) | ftxui::center,
        }) | ftxui::border;
    });
    
    return renderer;
}

// Function to be called from receive thread
void AddReceivedMessage(const std::string& sender, const std::string& content) {
    {
        std::lock_guard<std::mutex> lock(msg_mutex);
        messages.push_back({sender, content, false});
    }
    
    active_screen->Post(ftxui::Event::Custom);
    // Trigger UI refresh (thread-safe)
    // if (active_screen != nullptr) {
    // }
}

} // namespace UI
