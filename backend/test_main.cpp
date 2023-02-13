#include <iostream>
#include "./testing.h/testing.h"
#include "./config/test_config.h"

SUB_TEST(__main, {&test_config, "Test ./config"})

int main()
{
    return __main() == 0 ? 0 : 1;
}
