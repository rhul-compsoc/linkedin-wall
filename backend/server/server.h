#pragma once
#include <string>
#include "../config/config.h"
#include "../model/auth.h"
#include "../mongoose/mongoose.h"

namespace Linkedin
{
class Server
{
public:
    Server(Config *config);
    ~Server();
    void send_404(struct mg_connection *c, struct mg_http_message *hm);
private:
    bool try_get_value(std::string key);
    bool is_config_good();
    void run();

    /// gets the log path from the config
    std::string get_bind_addr();
    bool subnet_check(char ip_addr[4]);

    /// Checks that the required things are defined in the config
    Config *config;
    Authentication *auth;
};
}
