#include "./config/config.h"
#include "./server/server.h"
#include "./testing.h/logger.h"

using namespace Linkedin;

#define CONFIG_FILE "config.conf"

int main()
{
    lprintf(LOG_INFO, "Reading config from %s\n", CONFIG_FILE);
    Config config = Config(CONFIG_FILE);
    Server server(&config);
    return 1;
}
