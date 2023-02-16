#pragma once
#include "../config/config.h"
#include <pqxx/pqxx>
#include <string>

namespace Linkedin
{

class DatabaseConnector
{
public:
    DatabaseConnector(Config *config);
    ~DatabaseConnector();
    /// This will test whether the connections to the database can be made.
    bool test_connection();
    /// This will make any tables that are not made already
    void make_tables();
    pqxx::connection *get_connection();
private:
    /// This is a helper function to get the connection string
    std::string get_connection_string();
    /// Shared reference to the configuration of the server
    Config *config;
};

}
