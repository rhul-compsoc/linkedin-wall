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
    bool try_get_value(std::string key);
    bool is_config_good();
    void send_404(struct mg_connection *c, struct mg_http_message *hm);
    void upgrade_to_ws(struct mg_connection *c, struct mg_http_message *hm);
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
