#include "./database.h"
#include "../testing.h/logger.h"
#include <stdexcept>

using namespace Linkedin;

DatabaseConnector::DatabaseConnector(Config *config)
{
    this->config = config;
}

DatabaseConnector::~DatabaseConnector()
{

}

/// This will test whether the connections to the database can be made.
bool DatabaseConnector::test_connection()
{
    try {
        this->get_connection();
    } catch(std::exception &e) {
        lprintf(LOG_ERROR, "Testing database connection failed - %s\n", e.what());
        return false;
    }
    return true;
}

pqxx::connection *DatabaseConnector::get_connection()
{
    return new pqxx::connection(this->get_connection_string());
}

/// This will make any tables that are not made already
void DatabaseConnector::make_tables()
{
    const static std::string tables_sql = "";
}

std::string DatabaseConnector::get_connection_string()
{
    std::string ret = "dbname = " + this->config->get_value(DB_NAME_KEY) +
                      " user = " + this->config->get_value(DB_USER_KEY) +
                      " password = " + this->config->get_value(DB_PASSWORD_KEY) +
                      " hostaddr = " + this->config->get_value(DB_HOST_KEY) +
                      " port = " + this->config->get_value(DB_PORT_KEY);
    return ret;
}
