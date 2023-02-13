#pragma once
#include <map>
#include <list>
#include <string>

namespace Linkedin
{

/// This is the abstract config element
class ConfigElement
{
public:
    ConfigElement();
    virtual std::string to_string();
};

class ConfigKeyValueElement : public ConfigElement
{
public:
    ConfigKeyValueElement(std::string key, std::string value);
    std::string to_string() override;
private:
    std::string key, value;
};

/// This is the config element for a comment in the form of #yo, my comment is well cool
class ConfigCommentElement : public ConfigElement
{
public:
    ConfigCommentElement(std::string comment);
    std::string to_string() override;
private:
    std::string comment;
};

class Config
{
public:
    /// Reads a config file,
    /// exceptions are thrown if it is invalid or, cannot be read.
    /// if the file does not exist, it is made with the default config, an editor for the
    /// config is then opened then, it is read.
    Config(std::string file_name);

    /// Returns the value of the value of a key
    /// exceptions are thrown on error.
    std::string get_value(std::string key);

    /// Sets and, saves the config with a new value,
    /// exceptions are thrown on error.
    void set_value(std::string key, std::string value);

    /// Writes the entire config file to an open file stream
    void write(FILE *file);

private:
    void create_defaults();
    std::map<std::string, std::string> lookup_table;
    std::list<ConfigElement> raw_config;

    std::string file_name;
};

};
