#pragma once
#include "./config.h"
#include <stdio.h>

namespace Linkedin
{
/// An iterator using the next_element() should check while (v != CONFIG_PARSER_ERROR),
/// but also check for CONFIG_PARSER_SKIP.
typedef enum config_parser_state_t {
    /// There was an error reading the line, or there is no line to read
    CONFIG_PARSER_ERROR,
    /// The line that was just read should be stored and, there are no more
    CONFIG_PARSER_EOF,
    /// The line that was just read should be skipped and, not stored
    CONFIG_PARSER_SKIP,
    /// The line that was just read should be stored and, there are more left
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
