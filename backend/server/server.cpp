#include "./server.h"
#include "./connection.h"
#include "../testing.h/logger.h"
#include "../mongoose/mongoose.h"
#include <string.h>
#include <stdexcept>
#include <sys/param.h>
#include <nlohmann/json.hpp>

using namespace Linkedin;

Server::Server(Config *config)
{
    this->config = config;
    if (!this->is_config_good()) {
        throw std::runtime_error("Invalid configuration for server");
    }

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    this->conn_lock = lock;
    this->auth = new Authentication(this->config);
    this->run();
}

Server::~Server()
{
    delete this->auth;
}

void Server::__event_handler(struct mg_connection *c,
                             int event,
                             void *ev_data,
                             void *fn_data)
{
    if (event == MG_EV_ACCEPT) {
        //Init connection struct
        Connection *conn = new Connection();
        c->fn_data = (void *) conn;
        this->current_connections.push_back(conn);
    } else {
        Connection *connection = (Connection *) c->fn_data;
        if (event == MG_EV_HTTP_MSG) {
            struct mg_http_message *hm = (struct mg_http_message *) ev_data;
            this->send_404(c, hm);
        } else if (event == MG_EV_POLL && connection != fn_data) {
            ConnectionContext *ctx = connection->get_context();
            if (ctx == nullptr) return;
            if (!ctx->has_message()) return;

            std::string msg = ctx->next_message();
            mg_ws_send(c, msg.c_str(), msg.size(), MG_EV_WS_MSG);
        } else if (event == MG_EV_CLOSE) {
            for (std::list<Connection *>::iterator it = this->current_connections.begin();
                    it != this->current_connections.end(); it++) {
                if (*it == connection) {
                    it = this->current_connections.erase(it);
                    break;
                }
            }

            delete connection;
        }
    }
}

static void event_handler(struct mg_connection *c,
                          int event,
                          void *ev_data,
                          void *fn_data)
{
    Server *server = (Server *) fn_data;
    pthread_mutex_lock(&server->conn_lock);
    server->__event_handler(c, event, ev_data, fn_data);
    pthread_mutex_unlock(&server->conn_lock);
}

void Server::send_404(struct mg_connection *c, struct mg_http_message *hm)
{
    char url[1024];
    size_t len = MIN(sizeof(url) - 1, hm->uri.len);
    strncpy(url, hm->uri.ptr, len);
    url[len] = 0;

    mg_http_reply(c, 404, "", "404 error - cannot find %s", url);
    lprintf(LOG_WARNING, "404 - Page Not Found at: %s\n", url);
}

void Server::upgrade_to_ws(struct mg_connection *c, struct mg_http_message *hm)
{
    mg_ws_upgrade(c, hm, this->status("Upgraded to websocket").c_str());
}

std::string Server::status(std::string message)
{
    nlohmann::json status_json;
    status_json["status"] = message;
    return status_json.dump();
}

void Server::run()
{
    lprintf(LOG_INFO, "LinkedIn Wall Backend\n"
            "-> by djpiper28 see %s for git repo.\n"
            "-> Version %s\n",
            REPO_URL,
            VERSION);

    lprintf(LOG_INFO, "Starting api server, listening on %s\n",
            this->get_bind_addr().c_str());

    struct mg_mgr mgr;
    struct mg_connection *c;

    mg_mgr_init(&mgr);

    c = mg_http_listen(&mgr,
                       this->get_bind_addr().c_str(),
                       &event_handler,
                       (void *) this);

    if (c == NULL) {
        lprintf(LOG_ERROR, "Unable to init mongoose.\n");
        throw std::runtime_error("Cannot start mongoose");
    }

    for (;;) {
        mg_mgr_poll(&mgr, 100);
    }

    mg_mgr_free(&mgr);
}

#define CONFIG_ASSERT(x) if (!(x)) { lprintf(LOG_ERROR, "Cannot assert " #x " to be true - fatal configruation error\n"); return false; }

bool Server::try_get_value(std::string key)
{
    bool flag = true;
    try {
        this->config->get_value(key);
    } catch (std::exception &e) {
        flag = false;
    }
    return flag;
}

bool Server::is_config_good()
{
    CONFIG_ASSERT(this->try_get_value(BIND_ADDR_KEY));
    CONFIG_ASSERT(this->try_get_value(AUTH_KEY_KEY));
    CONFIG_ASSERT(this->try_get_value(CLIENT_ID_KEY));
    CONFIG_ASSERT(this->try_get_value(DB_NAME_KEY));
    CONFIG_ASSERT(this->try_get_value(DB_USER_KEY));
    CONFIG_ASSERT(this->try_get_value(DB_PASSWORD_KEY));
    CONFIG_ASSERT(this->try_get_value(DB_HOST_KEY));
    CONFIG_ASSERT(this->try_get_value(DB_PASSWORD_KEY));
    return true;
}

std::string Server::get_bind_addr()
{
    return this->config->get_value(BIND_ADDR_KEY);
}

bool Server::subnet_check(char ip_addr[4])
{
    return true;
}
