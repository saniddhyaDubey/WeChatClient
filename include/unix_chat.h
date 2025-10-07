#include <mutex>
#include <termios.h>
#include <string>
#include <iostream>
#include <unistd.h> 
#include <sstream>
#include "config.h"

namespace CD {
    extern struct termios old_tio, new_tio;
    extern std::string user_msg_buffer;
    extern std::string user_cmd_buffer;
    extern std::string rule_book;
    extern std::mutex console_mutex;

    void init();
    void cleanUpTio();

    void sendData();
    void receiveData();
}
