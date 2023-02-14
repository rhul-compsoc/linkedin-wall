#include "./server.h"
#include "./connection.h"
#include "../testing.h/logger.h"
#include "../mongoose/mongoose.h"
#include <string.h>
#include <stdexcept>
#include <sys/param.h>

using namespace Linkedin;

Server::Server(Config *config)
{
    this->config = config;
    if (!this->is_config_good()) {
        throw std::runtime_error("Invalid configuration for server");
    }

    this->auth = new Authentication(this->config);

    this->run();
}

Server::~Server()
{
    delete this->auth;
}

static void event_handler(struct mg_connection *c,
                          int event,
                          void *ev_data,
                          void *fn_data)
{
    Server *server = (Server *) fn_data;

    if (event == MG_EV_ACCEPT) {
        //Init connection struct
        c->fn_data = (void *) new Connection();
    } else {
        Connection *connection = (Connection *) c->fn_data;
        if (event == MG_EV_HTTP_MSG) {
            struct mg_http_message *hm = (struct mg_http_message *) ev_data;
            server->send_404(c, hm);
        } else if (event == MG_EV_CLOSE) {
            delete connection;
        }
    }
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

void Server::run()
{
    lprintf(LOG_INFO, "LinkedIn Wall Backend\n-> by djpiper28 see %s for git repo.\n",
            REPO_URL);
    printf("-> Version %s\n",
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
