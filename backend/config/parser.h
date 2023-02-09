#pragma once
#include "./config.h"
#include <stdio.h>

namespace Linkedin
{
typedef enum config_parser_state_t {
    CONFIG_PARSER_ERROR,
    CONFIG_PARSER_EOF,
    CONFIG_PARSER_NEXT=0,
} config_parser_state_t;

/// An LL-1 parser for a configuration file
class ConfigParser
{
public:
    ConfigParser(FILE *file);
    config_parser_state_t next_element(ConfigElement &next);
private:
    config_parser_state_t read_comment(ConfigElement &next);
    config_parser_state_t read_key_value(ConfigElement &next);
    FILE *file;
};
};
