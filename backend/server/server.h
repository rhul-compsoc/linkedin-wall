#pragma once
#include <pthread.h>
#include <string>
#include <list>
#include "../config/config.h"
#include "../model/auth.h"
#include "../mongoose/mongoose.h"
#include "./connection.h"

namespace Linkedin
{
class Server
{
public:
    Server(Config *config);
    ~Server();
    void __event_handler(struct mg_connection *c,
                         int event,
                         void *ev_data,
                         void *fn_data);

    /// This is used to lock the current connections, as this is mostly unlocked
    /// it should not hit performance too badly
    pthread_mutex_t conn_lock;
private:
    /// This method will try and get a config value and, is used by is_config_good()
    bool try_get_value(std::string key);

    /// This is a helper method used on startup to check the configuration
    bool is_config_good();

    /// This method will send a 404 error
    void send_404(struct mg_connection *c, struct mg_http_message *hm);

    /// This is a helper function to upgrade the connection to a web socket
    void upgrade_to_ws(struct mg_connection *c, struct mg_http_message *hm);

    /// This method generates a status message in json format {"satus":"message"}
    std::string status(std::string message);
    void run();

    /// gets the log path from the config
    std::string get_bind_addr();
    bool subnet_check(char ip_addr[4]);

    /// Checks that the required things are defined in the config
    Config *config;
    Authentication *auth;
    std::list<Connection *> current_connections;
};
}
