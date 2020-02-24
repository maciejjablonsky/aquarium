//
// Created by foreverhungry on 20.02.2020.
//

#include "config_parser.h"
#include "../object.h"
#include "../memory_handling.h"
#include <libxml/parser.h>
#include <libxml/xmlstring.h>
#include <string.h>
#include <libxml/xpath.h>
#include <ctype.h>
#include "exit_codes.h"

typedef enum {
    CONFIG_PARSER_NO_MEMORY, CONFIG_PARSER_XML_NOT_PARSED_FAIL, CONFIG_PARSER_EMPTY_XML_FAIL,
    CONFIG_PARSER_CONTEXT_FAIL
} config_parser_error_code_t;

static config_parser_t *delete_failed_config_parser(config_parser_t *this, config_parser_error_code_t error_code);

static bool is_parsed(xmlDoc *doc);

static bool is_not_parsed(xmlDoc *doc);

static const xmlChar * get_current_node_name(config_parser_t * this);
static xmlChar * trim_outer_whitespace(xmlChar * string);

config_parser_t *new_config_parser(const char *xml_filename) {
    config_parser_t *this = new_object(sizeof(config_parser_t));
    if (is_not_created(this)) {
        return delete_failed_config_parser(this, CONFIG_PARSER_NO_MEMORY);
    }
    this->doc = xmlParseFile(xml_filename);
    if (is_not_parsed(this->doc)) {
        return delete_failed_config_parser(this, CONFIG_PARSER_XML_NOT_PARSED_FAIL);
    }
    this->context = xmlXPathNewContext(this->doc);
    if (is_not_created(this->context)) {
        return delete_failed_config_parser(this, CONFIG_PARSER_CONTEXT_FAIL);
    }
    xmlXPathSetContextNode(xmlDocGetRootElement(this->doc), this->context);
    return this;
}

config_parser_t *delete_config_parser(config_parser_t *this) {
    if (is_not_created(this)) { return NULL; }
    if (is_created(this->doc)) {
        xmlFreeDoc(this->doc);
        this->doc = NULL;
    }
    if (is_created(this->context)) {
        xmlXPathFreeContext(this->context);
        this->context = NULL;
    }
    return delete_object(this);
}

xmlChar * trim_outer_whitespace(xmlChar * string) {
    size_t start, end;
    start = end = 0;
    while (isspace(string[start])) {start++;}
    end = xmlStrlen(string) - 1;
    while (isspace(string[end])) {end--;}
    xmlChar * trimmed = calloc(end - start + 2, sizeof(xmlChar));
    xmlStrncat(trimmed, string + start, end - start + 1);
    return trimmed;
}


xmlChar *config_xml_get_current_value(config_parser_t *this) {
    xmlChar * value = xmlNodeListGetString(this->doc, this->context->node->xmlChildrenNode, 1);
    if (value == NULL) {return NULL;}
    xmlChar * trimmed = trim_outer_whitespace(value);
    xmlFree(value);
    return trimmed;
}

xmlChar * config_xml_get_value_with_xpath(config_parser_t * this, char *xpath) {
    xmlNode * start_node = this->context->node;
    config_xml_eval_xpath(this, TO_XML_STRING(xpath));
    xmlChar * value = config_xml_get_current_value(this);
    xmlXPathSetContextNode(start_node, this->context);
    return value;
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
        case CONFIG_PARSER_CONTEXT_FAIL:
            IMPLICIT_ERROR_MESSAGE("Failed to create XPath context for xml config file.");
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

const xmlChar * get_current_node_name(config_parser_t * this) {
    if (this->context->node) {
        return this->context->node->name;
    }
    return NULL;
}

bool config_xml_eval_xpath(config_parser_t *this, const xmlChar *xpath) {
    xmlXPathObject * result = xmlXPathEvalExpression(xpath, this->context);
    if (is_not_created(result)) {
        IMPLICIT_ERROR_MESSAGE("Error in xmlXPathEvalExpression.");
        exit(EXIT_XML_PARSING_ERROR);
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
        xmlXPathFreeObject(result);
        IMPLICIT_ERROR_MESSAGE("Node [%s] is not found.", xpath);
        exit(EXIT_XML_PARSING_ERROR);
    }
    if (result->nodesetval->nodeNr == 1) {
        xmlXPathSetContextNode(result->nodesetval->nodeTab[0], this->context);
        xmlXPathFreeObject(result);
        return true;
    } else {
        IMPLICIT_ERROR_MESSAGE("Cannot go to multiple nodes.");
        xmlXPathFreeObject(result);
        exit(EXIT_XML_PARSING_ERROR);
    }
}







