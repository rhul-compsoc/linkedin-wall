#include "./parser.h"
#include "../testing.h/logger.h"
#include <assert.h>

using namespace Linkedin;

ConfigParser::ConfigParser(FILE *file)
{
    this->file = file;
    assert(file != NULL);
}

std::pair<config_parser_state_t, ConfigElement *> ConfigParser::read_comment()
{
    std::string buffer = "";

    int c = EOF;
    while (c = fgetc(this->file), c != EOF) {
        if (c == '\n') {
            break;
        }
        buffer += c;
    }

    return std::pair(CONFIG_PARSER_NEXT, new ConfigCommentElement(buffer));
}

std::pair<config_parser_state_t, ConfigElement *> ConfigParser::read_key_value()
{
    bool seen_value = false;
    std::string key = "";
    std::string value = "";

    int c = EOF;
    while (c = fgetc(this->file), c != EOF) {
        if (c == '\n') {
            break;
        }

        if (c == '=' && !seen_value) {
            seen_value = true;
        } else if (!seen_value) {
            key += c;
        } else {
            value += c;
        }
    }

    if (value.size() == 0) {
        lprintf(LOG_ERROR, "Found a zero-length value for key %s\n", key.c_str());
        return std::pair(CONFIG_PARSER_ERROR, nullptr);
    }

    if (key.size() == 0) {
        lprintf(LOG_ERROR, "Found a zero-length key\n");
        return std::pair(CONFIG_PARSER_ERROR, nullptr);
    }

    return std::pair(CONFIG_PARSER_NEXT, new ConfigKeyValueElement(key, value));
}

std::pair<config_parser_state_t, ConfigElement *> ConfigParser::next_element()
{
    int c = fgetc(this->file);
    if (c == EOF) {
        return std::pair(CONFIG_PARSER_EOF, nullptr);
    } else if (c == '\n') {
        lprintf(LOG_WARNING, "Empty line found in config file, this line will be removed\n");
        return std::pair(CONFIG_PARSER_SKIP, nullptr);
    } else if (c == '#') {
        return this->read_comment();
    }

    ungetc(c, this->file);
    return this->read_key_value();
}

