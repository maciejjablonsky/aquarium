#ifndef AQUARIUM_CONFIG_PARSER_H
#define AQUARIUM_CONFIG_PARSER_H
#include <libxml/tree.h>

typedef struct {
    xmlDoc * doc;
    xmlNode * root;
} config_parser_t;
#define CONFIG_PARSER_T_NAME "config_parser_t"

config_parser_t * new_config_parser(const char * xml_filename);
config_parser_t * delete_config_parser(config_parser_t * this);


#endif //AQUARIUM_CONFIG_PARSER_H
