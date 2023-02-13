#include "./parser.h"
#include "./config.h"
#include "../testing.h/logger.h"
#include <stdexcept>

using namespace Linkedin;

ConfigElement::ConfigElement()
{

}

std::string ConfigElement::to_string()
{
    lprintf(LOG_ERROR, "Virtual method called erroneously.\n");
    return "error";
}

ConfigKeyValueElement::ConfigKeyValueElement(std::string key, std::string value)
{
    this->key = key;
    this->value = value;
}

std::string ConfigKeyValueElement::to_string()
{
    return this->key + "=" + this->value;
}

ConfigCommentElement::ConfigCommentElement(std::string comment)
{
    this->comment = comment;
}

std::string ConfigCommentElement::to_string()
{
    return "#" + this->comment;
}

Config::Config(std::string file_name)
{
    this->file_name = file_name;

    FILE *f = fopen(file_name.c_str(), "r");
    if (f == NULL) {
        this->create_defaults();
    } else {
        ConfigParser parser(f);
        // TODO: Parse the file
        fclose(f);
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

void Config::write(FILE *file)
{
    for (ConfigElement element : this->raw_config) {
        fprintf(file, "%s\n", element.to_string().c_str());
    }
}
