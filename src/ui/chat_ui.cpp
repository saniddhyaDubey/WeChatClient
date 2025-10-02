#include "ui/chat_ui.h"

namespace UI {

void ChatUI::Run() {
    bool should_exit = false;
    
    while (!should_exit) {
        ftxui::Component component;
        
        switch (current_state) {
            case AppState::MENU:
                component = RenderMenuScreen(*this);
                break;
            case AppState::LOGIN:
                component = RenderLoginScreen(*this);
                break;
            case AppState::REGISTER:
                component = RenderRegisterScreen(*this);
                break;
            case AppState::CHAT_LOBBY:
                component = RenderChatLobbyScreen(*this);
                break;
            case AppState::CHATTING:
                component = RenderChattingScreen(*this);
                break;
            default:
                return;
        }
        
        AppState state_before_loop = current_state;
        screen.Loop(component);
        
        // Only exit if we're at MENU and state didn't change (user clicked Quit)
        if (current_state == AppState::MENU && state_before_loop == AppState::MENU) {
            should_exit = true;
        }
    }
}

ftxui::Element ChatUI::RenderCurrentScreen() {
    return ftxui::text("");
}

} // namespace UI
