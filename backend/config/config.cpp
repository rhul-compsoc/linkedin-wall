#include "./parser.h"
#include "./config.h"
#include "../testing.h/logger.h"
#include <stdexcept>
#include <type_traits>

using namespace Linkedin;

/// Define base elemeent
ConfigElement::ConfigElement()
{

}

ConfigElement::~ConfigElement()
{

}

std::string ConfigElement::to_string()
{
    lprintf(LOG_ERROR, "Virtual method called erroneously.\n");
    throw std::runtime_error("to_string() not implemented for ConfigElement");
}

config_element_type_t ConfigElement::type()
{
    return CONFIG_ELEMENT_OTHER;
}

/// Override config key value element
ConfigKeyValueElement::ConfigKeyValueElement(std::string key, std::string value)
{
    this->key = key;
    this->value = value;
}

std::string ConfigKeyValueElement::to_string()
{
    return this->key + "=" + this->value;
}

config_element_type_t ConfigKeyValueElement::type()
{
    return CONFIG_ELEMENT_KEY_VALUE;
}

/// Override comment element
ConfigCommentElement::ConfigCommentElement(std::string comment)
{
    this->comment = comment;
}

std::string ConfigCommentElement::to_string()
{
    return "#" + this->comment;
}

config_element_type_t ConfigCommentElement::type()
{
    return CONFIG_ELEMENT_COMMENT;
}

/// Define config constructor, the file_name is written to on to_string
Config::Config(std::string file_name)
{
    this->file_name = file_name;

    FILE *f = fopen(file_name.c_str(), "r");
    if (f == NULL) {
        this->create_defaults();
    } else {
        ConfigParser parser = ConfigParser(f);
        bool flag = true;
        size_t line = 0;

        while (flag) {
            line++;
            std::pair<config_parser_state_t, ConfigElement *> ret = parser.next_element();

            // This allows for empty lines to be skipped
            if (ret.first == CONFIG_PARSER_SKIP) {
                continue;
            }

            // Check for errors
            if (ret.first == CONFIG_PARSER_ERROR) {
                lprintf(LOG_ERROR, "An error has occurred on line %ld of the config %s\n",
                        line,
                        file_name.c_str());
                fclose(f);
                throw std::runtime_error("An error occurred during parsing the config file");
            }

            // Check for EOF
            if (ret.first == CONFIG_PARSER_EOF) {
                flag = false;
                continue;
            }

            this->handle_token(ret.second);
        }
        fclose(f);
    }
}

Config::~Config()
{
    for(ConfigElement *el : this->raw_config) {
        delete el;
    }
}

void Config::create_defaults()
{
    FILE *f = fopen(this->file_name.c_str(), "w");
    fprintf(f, "# Authentication key for oauth, this can be set by the server in runtime\n"
            "# DON'T CHANGE BY HAND\n" \
            "auth_key=nan\n");
    fclose(f);
}

std::string Config::get_value(std::string key)
{
    if (auto search = this->lookup_table.find(key); search != lookup_table.end()) {
        return search->first;
    } else {
        lprintf(LOG_ERROR, "Cannot find key %s in config\n", key.c_str());
        throw std::runtime_error("Cannot find key in config");
    }
}

void Config::set_value(std::string key, std::string value)
{
    std::string old_value = this->get_value(key);
    if (old_value == value) {
        lprintf(LOG_INFO, "Value being set is the same, no file write will occur\n");
        return;
    }

    this->lookup_table[key] = value;
    FILE *f = fopen(this->file_name.c_str(), "w");
    this->write(f);
    fclose(f);
}

void Config::handle_token(ConfigElement *token)
{
    if (token == nullptr) {
        lprintf(LOG_ERROR, "Null token in config reader\n");
        throw std::runtime_error("Null token in config reader");
    }

    if (token->type() == CONFIG_ELEMENT_KEY_VALUE) {
        ConfigKeyValueElement *token_kv = dynamic_cast<ConfigKeyValueElement *>(token);
        if (auto search = this->lookup_table.find(token_kv->key); search != lookup_table.end()) {
            this->lookup_table[token_kv->key] = token_kv->value;
        } else {
            lprintf(LOG_ERROR, "Key %s has been defined more than once in config file\n",
                    token_kv->key.c_str());
            throw std::runtime_error("Key defined more than once in config file");
        }
    }

    this->raw_config.push_back(token);
}

void Config::write(FILE *file)
{
    for (ConfigElement *element : this->raw_config) {
        fprintf(file, "%s\n", element->to_string().c_str());
    }
}
