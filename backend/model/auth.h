#pragma once
#include <string>

namespace Linkedin
{

/// This is an abstractino of the authorisation and, authentication systems that
/// Linkedin uses. https://learn.microsoft.com/en-us/linkedin/consumer/integrations/self-serve/sign-in-with-linkedin-v2
class Authentication
{
public:
    /// Inits the authentication object with the authentication token
    Authentication(std::string token);

    /// This method will open an Oauth2 login page on your browser
    /// Throws an exception if this fails
    Authentication();

    /// Returns the auth header string (in form Authorization: Bearer <access token>)
    std::string get_auth_header();

    /// Returns the authorisation token, this is useful for updating the authorisaiton cache
    std::string get_token();

private:
    std::string token;
};

};
