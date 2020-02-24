#ifndef AQUARIUM_CONFIG_PARSER_H
#define AQUARIUM_CONFIG_PARSER_H
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <stdbool.h>
#define TO_XML_STRING(STR) ((const xmlChar*)(STR))
#define XML_PARSING_ERROR 3

typedef struct {
    xmlDoc * doc;
    xmlXPathContext * context;
} config_parser_t;
#define CONFIG_PARSER_T_NAME "config_parser_t"

config_parser_t * new_config_parser(const char * xml_filename);
config_parser_t * delete_config_parser(config_parser_t * this);
bool config_xml_go_to_parent(config_parser_t * this);
bool config_xml_go_to_sibling_named(config_parser_t *this, const xmlChar *name);
xmlChar * config_xml_get_current_value(config_parser_t * this);
bool config_xml_eval_xpath(config_parser_t * this, const xmlChar *xpath);
xmlChar * config_xml_get_value_with_xpath(config_parser_t * this, char *xpath);


#endif //AQUARIUM_CONFIG_PARSER_H
