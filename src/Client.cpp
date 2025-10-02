#include <iostream>
#include "ui/chat_ui.h"

int main()
{
    std::cout << "Starting UI..." << std::endl;
    UI::ChatUI ui;
    std::cout << "UI created, calling Run()..." << std::endl;
    ui.Run();
    std::cout << "UI exited." << std::endl;
    return 0;
}
