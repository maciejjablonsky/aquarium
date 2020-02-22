//
// Created by foreverhungry on 20.02.2020.
//

#include "config_parser.h"
#include "../object.h"
#include "../memory_handling.h"
#include <libxml/parser.h>

typedef enum {
    CONFIG_PARSER_NO_MEMORY, CONFIG_PARSER_XML_NOT_PARSED_FAIL, CONFIG_PARSER_EMPTY_XML_FAIL
} config_parser_error_code_t;

static config_parser_t *delete_failed_config_parser(config_parser_t *this, config_parser_error_code_t error_code);

static bool is_parsed(xmlDoc *doc);

static bool is_not_parsed(xmlDoc *doc);

config_parser_t *new_config_parser(const char *xml_filename) {
    config_parser_t *this = new_object(sizeof(config_parser_t));
    if (is_not_created(this)) {
        return delete_failed_config_parser(this, CONFIG_PARSER_NO_MEMORY);
    }
    this->doc = xmlParseFile(xml_filename);
    if (is_not_parsed(this->doc)) {
        EXPLICIT_ERROR_MESSAGE("Config file: %s", xml_filename);
        return delete_failed_config_parser(this, CONFIG_PARSER_XML_NOT_PARSED_FAIL);
    }
    this->root = xmlDocGetRootElement(this->doc);
    if (this->root == NULL) {
        return delete_failed_config_parser(this, CONFIG_PARSER_EMPTY_XML_FAIL);
    }
    return this;
}

config_parser_t *delete_config_parser(config_parser_t *this) {
    if (is_not_created(this)) { return NULL; }
    if (is_created(this->doc)) {
        xmlFreeDoc(this->doc);
        this->doc = NULL;
    }
    if (is_created(this->root)) {
        xmlFreeNode(this->root);
    }
    return delete_object(this);
}

static config_parser_t *delete_failed_config_parser(config_parser_t *this, config_parser_error_code_t error_code) {
    switch (error_code) {
        case CONFIG_PARSER_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case CONFIG_PARSER_XML_NOT_PARSED_FAIL:
        EXPLICIT_ERROR_MESSAGE("Failed to parse xml config file.");
            break;
        case CONFIG_PARSER_EMPTY_XML_FAIL:
        EXPLICIT_ERROR_MESSAGE("Given configuration file is empty.");
            break;
        default:
            break;
    }
    return delete_config_parser(this);
}

bool is_parsed(xmlDoc *doc) {
    return doc != NULL;
}

bool is_not_parsed(xmlDoc *doc) {
    return !is_parsed(doc);
}




