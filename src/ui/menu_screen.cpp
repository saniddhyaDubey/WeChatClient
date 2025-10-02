#include "ui/chat_ui.h"

namespace UI {

ftxui::Component RenderMenuScreen(ChatUI& ui) {
    auto register_button = ftxui::Button("Register", [&ui] {
        ui.current_state = AppState::REGISTER;
        ui.GetScreen().ExitLoopClosure()();
    });
    
    auto login_button = ftxui::Button("Login", [&ui] {
        ui.current_state = AppState::LOGIN;
        ui.GetScreen().ExitLoopClosure()();
    });
    
    auto quit_button = ftxui::Button("Quit", [&ui] {
        ui.GetScreen().ExitLoopClosure()();
    });
    
    auto button_container = ftxui::Container::Vertical({
        register_button,
        login_button,
        quit_button
    });
    
    auto renderer = ftxui::Renderer(button_container, [=] {
        return ftxui::vbox({
            ftxui::text("") | ftxui::center,
            ftxui::text("═════════════════════════════════════════════════") | ftxui::bold | ftxui::center,
            ftxui::text("WELCOME TO WECHAT") | ftxui::bold | ftxui::center,
            ftxui::text("═════════════════════════════════════════════════") | ftxui::bold | ftxui::center,
            ftxui::text(""),
            ftxui::text("Instructions:") | ftxui::bold,
            ftxui::text("  Register : Create a new account"),
            ftxui::text("  Login    : Login to existing account"),
            ftxui::text("  Quit     : Exit application"),
            ftxui::text(""),
            ftxui::separator(),
            ftxui::text(""),
            register_button->Render() | ftxui::center,
            login_button->Render() | ftxui::center,
            quit_button->Render() | ftxui::center,
        }) | ftxui::border | ftxui::center;
    });
    
    return renderer;
}

} // namespace UI
