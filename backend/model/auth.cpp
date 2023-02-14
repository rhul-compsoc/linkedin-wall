#include "./auth.h"
#include "../testing.h/logger.h"

using namespace Linkedin;

Authentication::Authentication(Config *config)
{
    this->config = config;

    if (!this->test_auth()) {
        this->refresh_authentication();
    }
}

std::string Authentication::get_auth_header()
{
    return "Authorization: Bearer " + this->get_token();
}

std::string Authentication::get_token()
{
    return this->config->get_value(AUTH_KEY_KEY);
}

std::string Authentication::get_client_id()
{
    return this->config->get_value(CLIENT_ID_KEY);
}

std::string Authentication::get_auth_url()
{
    return "https://www.linkedin.com/developers/tools/oauth/token-generator?clientId=" + this->get_client_id();
}

void Authentication::refresh_authentication()
{
    lprintf(LOG_WARNING, "You must use Oauth2 to authorise this web-app.\n"
            "Please go to %s\n"
            "then set the auth key in the web interface or config file.\n",
            this->get_auth_url().c_str());
}

bool Authentication::test_auth()
{
    return false;
}
