#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

namespace UI {

enum class AppState {
    MENU,
    LOGIN,
    REGISTER,
    CHAT_LOBBY,
    CHATTING
};

class ChatUI {
public:
    void Run();
    
    AppState current_state = AppState::MENU;
    std::string username;
    std::string password;
    std::string secret_key;
    bool is_logged_in = false;
    std::string current_recipient;
    
    ftxui::ScreenInteractive& GetScreen() { return screen; }
    
private:
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    ftxui::Component main_component;
    
    ftxui::Element RenderCurrentScreen();
};

ftxui::Component RenderMenuScreen(ChatUI& ui);
ftxui::Component RenderLoginScreen(ChatUI& ui);
ftxui::Component RenderRegisterScreen(ChatUI& ui);
ftxui::Component RenderChatLobbyScreen(ChatUI& ui);
ftxui::Component RenderChattingScreen(ChatUI& ui);
void AddReceivedMessage(const std::string& sender, const std::string& content);

} // namespace UI

#endif // CHAT_UI_H
