#include "./config.h"
#include "./parser.h"
#include "../testing.h/testing.h"
#include <stdio.h>
#include <string>
#include <unistd.h>

using namespace Linkedin;

#define KEY_VALUE_1 "key=value"
#define COMMENT_1 "# Lorem ipsum dolor sit amet, qui minim labore adipisicing minim sint cillum sint consectetur cupidatat."

static int test_config_elemenets()
{
    ConfigKeyValueElement test1 = ConfigKeyValueElement("key", "value");
    ASSERT(test1.to_string() == "key=value");

    ConfigCommentElement test2 = ConfigCommentElement(std::string(COMMENT_1).substr(1));
    ASSERT(test2.to_string() == COMMENT_1);

    return 1;
}

static int test_parser_good_case()
{
    int fid[2];
    ASSERT(pipe(fid) == 0);

    FILE *r = fdopen(fid[0], "r");
    ASSERT(r != NULL);

    FILE *w = fdopen(fid[1], "w");
    ASSERT(w != NULL);

    ConfigParser parser(r);

    // Test key=value
    fprintf(w, KEY_VALUE_1 "\n");
    fflush(w);

    std::pair<config_parser_state_t, ConfigElement *> ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_NEXT);
    ASSERT(ret.second->to_string() == KEY_VALUE_1);

    // Test key=value
    fprintf(w, KEY_VALUE_1 "\n");
    fflush(w);

    ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_NEXT);
    ASSERT(ret.second->to_string() == KEY_VALUE_1);

    // Test a comment
    fprintf(w, COMMENT_1 "\n");
    fflush(w);

    ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_NEXT);
    ASSERT(ret.second->to_string() == COMMENT_1);

    // Test key=value
    fprintf(w, KEY_VALUE_1 "\n");
    fflush(w);

    ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_NEXT);
    ASSERT(ret.second->to_string() == KEY_VALUE_1);

    // Test new lines are ignored
    fprintf(w, "\n");
    fflush(w);

    ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_SKIP);

    // Test EOF
    ASSERT(fclose(w) == 0);
    ret = parser.next_element();

    ASSERT(ret.first == CONFIG_PARSER_EOF);

    // Cleanup
    ASSERT(fclose(r) == 0);
    return 1;
}

static int test_parser_error_case_1()
{
    int fid[2];
    ASSERT(pipe(fid) == 0);

    FILE *r = fdopen(fid[0], "r");
    ASSERT(r != NULL);

    FILE *w = fdopen(fid[1], "w");
    ASSERT(w != NULL);

    ConfigParser parser(r);

    fprintf(w, "le fische au kerosine\n");
    fflush(w);

    std::pair<config_parser_state_t, ConfigElement *> ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_ERROR);

    // Cleanup
    ASSERT(fclose(w) == 0);
    ASSERT(fclose(r) == 0);
    return 1;
}

static int test_parser_error_case_2()
{
    int fid[2];
    ASSERT(pipe(fid) == 0);

    FILE *r = fdopen(fid[0], "r");
    ASSERT(r != NULL);

    FILE *w = fdopen(fid[1], "w");
    ASSERT(w != NULL);

    ConfigParser parser(r);

    fprintf(w, "=le fische au kerosine\n");
    fflush(w);

    std::pair<config_parser_state_t, ConfigElement *> ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_ERROR);

    // Cleanup
    ASSERT(fclose(w) == 0);
    ASSERT(fclose(r) == 0);
    return 1;
}

static int test_parser_error_case_3()
{
    int fid[2];
    ASSERT(pipe(fid) == 0);

    FILE *r = fdopen(fid[0], "r");
    ASSERT(r != NULL);

    FILE *w = fdopen(fid[1], "w");
    ASSERT(w != NULL);

    ConfigParser parser(r);

    fprintf(w, "le fische au kerosine=\n");
    fflush(w);

    std::pair<config_parser_state_t, ConfigElement *> ret = parser.next_element();
    ASSERT(ret.first == CONFIG_PARSER_ERROR);

    // Cleanup
    ASSERT(fclose(w) == 0);
    ASSERT(fclose(r) == 0);
    return 1;
}

#define GOOD_CONFIG_FILE "good_config.conf"
#define GOOD_CONFIG_FILE_2 "2_" GOOD_CONFIG_FILE

static int test_config_good_case()
{
    FILE *f = fopen(GOOD_CONFIG_FILE, "w");
    ASSERT(f != NULL);
    fprintf(f, "# This is a test file\n"
            "abc=123\n"
            KEY_VALUE_1 "\n"
            COMMENT_1 "\n"
            "\n\n\n");
    fclose(f);

    Config config = Config(GOOD_CONFIG_FILE);

    ASSERT(config.get_value("abc") == "123");
    config.set_value("abc", "123");
    ASSERT(config.get_value("abc") == "123");

    config.set_value("abc", "1234");
    ASSERT(config.get_value("abc") == "1234");

    lprintf(LOG_INFO, "Write the file then test it can be read\n");
    f = fopen(GOOD_CONFIG_FILE_2, "w");
    config.write(f);
    fclose(f);

    Config config2 = Config(GOOD_CONFIG_FILE_2);
    ASSERT(config2.get_value("abc") == "1234");

    return 1;
}

SUB_TEST(test_config, {&test_config_elemenets, "Test config elements"},
{&test_parser_good_case, "Test parser good case"},
{&test_parser_error_case_1, "Test parser error case 1"},
{&test_parser_error_case_2, "Test parser error case 2"},
{&test_parser_error_case_3, "Test parser error case 3"},
{&test_config_good_case, "Test config good case"})
