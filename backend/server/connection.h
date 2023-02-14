#pragma once
#include <time.h>

namespace Linkedin
{

class Connection
{
public:
    Connection();
private:
    time_t connection_time;
};

}
