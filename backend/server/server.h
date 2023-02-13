#pragma once
#include <string>
#include "../config/config.h"

namespace Linkedin
{
class Server
{
public:
    Server(Config *config);
    ~Server();
private:
    bool try_get_value(std::string key);
    bool is_config_good();
    void run();
    /// Checks that the required things are defined in the config
    Config *config;

    /// gets the log path from the config
    std::string get_bind_addr();
    bool subnet_check(char ip_addr[4]);
};
}
