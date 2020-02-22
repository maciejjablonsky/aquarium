#ifndef AQUARIUM_CONFIG_PARSER_H
#define AQUARIUM_CONFIG_PARSER_H
#include <libxml/tree.h>
#include <stdbool.h>
#define TO_XML_STRING(NAME) ((const xmlChar*)(NAME))
#define XML_PARSING_ERROR 3

typedef struct {
    xmlDoc * doc;
    xmlNode * root;
    xmlNode * current_node;
} config_parser_t;
#define CONFIG_PARSER_T_NAME "config_parser_t"

config_parser_t * new_config_parser(const char * xml_filename);
config_parser_t * delete_config_parser(config_parser_t * this);
bool config_xml_go_to_child_named(config_parser_t *this, const xmlChar *name);
xmlNode * get_config_xml_root(config_parser_t * this);
bool config_xml_go_up_to_parent(config_parser_t * this);
bool config_xml_go_to_next_sibling(config_parser_t * this);
bool config_xml_go_to_first_child(config_parser_t * this);
bool config_xml_go_to_sibling_named(config_parser_t *this, const xmlChar *name);
xmlChar * config_xml_get_current_value(config_parser_t * this);
bool config_xml_go_to_node_by_path(config_parser_t * this, const xmlChar *path);


#endif //AQUARIUM_CONFIG_PARSER_H
