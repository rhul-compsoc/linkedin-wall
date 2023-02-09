#include "./parser.h"
#include "./config.h"
#include "../testing.h/logger.h"

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
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        this->create_default(file_name);
    } else {
        ConfigParser parser(f);
        // TODO: Parse the file
        fclose(f);
    }
}

std::string Config::create_defaults(std::string file_name)
{

}

std::string Config::get_value(std::string key)
{

}

std::string Config::set_value(std::string key, std::string value)
{

}

void Config::write(FILE *file)
{

}
