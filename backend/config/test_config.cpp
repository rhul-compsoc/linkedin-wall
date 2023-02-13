#include "./config.h"
#include "./parser.h"
#include "../testing.h/testing.h"

static int test_parser_good_case()
{
    return 1;
}

SUB_TEST(test_config, {&test_parser_good_case, "Test parser good case"})
