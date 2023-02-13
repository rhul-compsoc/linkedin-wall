#pragma once
#include "./config.h"
#include <stdio.h>
#include <utility>

namespace Linkedin
{
/// An iterator using the next_element() should check while (v != CONFIG_PARSER_ERROR && v != CONFIG_PARSER_EOF),
/// but also check for CONFIG_PARSER_SKIP.
typedef enum config_parser_state_t {
    /// There was an error reading the line, or there is no line to read
    CONFIG_PARSER_ERROR,
    /// The config is at the end of file and, no data was read
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
    std::pair<config_parser_state_t, ConfigElement *> next_element();
private:
    std::pair<config_parser_state_t, ConfigElement *> read_comment();
    std::pair<config_parser_state_t, ConfigElement *> read_key_value();
    FILE *file;
};
};
