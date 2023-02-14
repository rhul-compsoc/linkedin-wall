#pragma once
#include <time.h>
#include <string>

namespace Linkedin
{

class ConnectionContext
{
public:
    ConnectionContext();
    virtual ~ConnectionContext();
    virtual bool has_message();
    virtual std::string next_message();
};

class Connection
{
public:
    Connection();
    ~Connection();
    void set_context(ConnectionContext *context);
    ConnectionContext *get_context();
private:
    time_t connection_time;
    ConnectionContext *context;
};

}
