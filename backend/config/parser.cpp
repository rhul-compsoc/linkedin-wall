#include "./parser.h"
#include <assert.h>

using namespace Linkedin;

ConfigParser::ConfigParser(FILE *file)
{
    this->file = file;
    assert(file != NULL);
}

config_parser_state_t ConfigParser::read_comment(ConfigElement &next)
{
    std::string buffer = "";

    int c = EOF;
    while (c = fgetc(this->file), c != EOF) {
        if (c == '\n') {
            break;
        }
        buffer += c;
    }

    ConfigCommentElement tmp = ConfigCommentElement(buffer);
    next = tmp;

    if (c == EOF) {
        return CONFIG_PARSER_EOF;
    } else {
        return CONFIG_PARSER_NEXT;
    }
}

config_parser_state_t ConfigParser::read_key_value(ConfigElement &next)
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
        return CONFIG_PARSER_ERROR;
    }

    ConfigKeyValueElement tmp = ConfigKeyValueElement(key, value);
    next = tmp;

    if (c == EOF) {
        return CONFIG_PARSER_EOF;
    } else {
        return CONFIG_PARSER_NEXT;
    }
}

config_parser_state_t ConfigParser::next_element(ConfigElement &next)
{
    int c = fgetc(this->file);
    if (c == EOF) {
        return CONFIG_PARSER_EOF;
    } else if (c == '\n') {
        return CONFIG_PARSER_SKIP;
    } else if (c == '#') {
        return this->read_comment(next);
    }

    ungetc(c, this->file);
    return this->read_key_value(next);
}

