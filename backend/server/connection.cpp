#include "./connection.h"
#include "../testing.h/logger.h"
#include <stdexcept>

using namespace Linkedin;

Connection::Connection()
{
    this->connection_time = time(NULL);
    this->context = nullptr;
}

Connection::~Connection()
{
    if (this->context!= nullptr) {
        delete this->context;
    }
}

void Connection::set_context(ConnectionContext *context)
{
    this->context = context;
}

ConnectionContext *Connection::get_context()
{
    return this->context;
}

ConnectionContext::ConnectionContext()
{

}

ConnectionContext::~ConnectionContext()
{

}

bool ConnectionContext::has_message()
{
    return false;
}

std::string ConnectionContext::next_message()
{
    lprintf(LOG_ERROR, "Cannot get next_message for unspecified context\n");
    throw std::runtime_error("Cannot get next_message for unspecified context\n");
}
