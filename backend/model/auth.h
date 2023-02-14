#pragma once
#include <string>
#include "../config/config.h"

namespace Linkedin
{

/// This is an abstractino of the authorisation and, authentication systems that
/// Linkedin uses. https://learn.microsoft.com/en-us/linkedin/consumer/integrations/self-serve/sign-in-with-linkedin-v2
class Authentication
{
public:
    /// Inits the authentication object with the authentication token
    Authentication(Config *config);

    /// Returns the auth header string (in form Authorization: Bearer <access token>)
    std::string get_auth_header();

    /// Returns the authorisation token, this is useful for updating the authorisaiton cache
    std::string get_token();

    /// This function will test to see if the authentication works, the server should poll this
    /// and, then request a refresh when it doesn't work.
    bool test_auth();

    /// This will get the Linkedin Oauth2 authentication URL
    std::string get_auth_url();
private:
    /// This method prints the authentication URL into console with instructions, it used on
    /// program start to tell the user to fix their config.
    void refresh_authentication();
    std::string get_client_id();
    Config *config;
};

};
