#include "./auth.h"
#include "../testing.h/logger.h"

using namespace Linkedin;

Authentication::Authentication(std::string token)
{
    this->token = token;
}

Authentication::Authentication()
{
    char url[] = "";

    // Open the LinkedIn Oauth2 page
    lprintf(LOG_INFO, "You must use Oauth2 to authorise this web-app.\n"
            "Please enter the following url %s into your machine.\n",
            url);

    // Waits for the server to get a response
    lprintf(LOG_INFO, "Once you have logged in via the page, the login "
            "process will continue.\n");
    /// TODO!!!!

    // Stores the token locally
    this->token = "CHANGE ME 123";
}

std::string Authentication::get_auth_header()
{
    return "Authorization: Bearer " + this->token;
}

std::string Authentication::get_token()
{
    return this->token;
}
