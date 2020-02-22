//
// Created by foreverhungry on 20.02.2020.
//

#include "config_parser.h"
#include "../object.h"
#include "../memory_handling.h"
#include <libxml/parser.h>
#include <libxml/xmlstring.h>
#include <string.h>

typedef enum {
    CONFIG_PARSER_NO_MEMORY, CONFIG_PARSER_XML_NOT_PARSED_FAIL, CONFIG_PARSER_EMPTY_XML_FAIL
} config_parser_error_code_t;

static config_parser_t *delete_failed_config_parser(config_parser_t *this, config_parser_error_code_t error_code);

static bool is_parsed(xmlDoc *doc);

static bool is_not_parsed(xmlDoc *doc);

static xmlNode *_go_to_sibling_named(xmlNode *current, const xmlChar *name);

static xmlChar *_next_node_name(const xmlChar *path);

static bool is_beginning_with_slash(xmlChar *path);
static int skip_slashes(const xmlChar *path);


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
    this->current_node = this->root;
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

bool config_xml_go_to_child_named(config_parser_t *this, const xmlChar *name) {
    xmlNode *current = _go_to_sibling_named(this->current_node->children, name);
    if (current != NULL) {
        this->current_node = current;
        return true;
    }
    return false;
}

xmlNode *get_config_xml_root(config_parser_t *this) {
    return this->root;
}

bool config_xml_go_up_to_parent(config_parser_t *this) {
    if (this->current_node->parent) {
        this->current_node = this->current_node->parent;
        return true;
    }
    return false;
}

bool config_xml_go_to_next_sibling(config_parser_t *this) {
    if (this->current_node->next) {
        this->current_node = this->current_node->next;
        return true;
    }
    return false;
}

bool config_xml_go_to_first_child(config_parser_t *this) {
    if (this->current_node->children) {
        this->current_node = this->current_node->children;
        return true;
    }
    return false;
}

bool config_xml_go_to_sibling_named(config_parser_t *this, const xmlChar *name) {
    xmlNode *current = _go_to_sibling_named(this->current_node, name);
    if (current != NULL) {
        this->current_node = current;
        return true;
    }
    return false;
}

xmlChar *config_xml_get_current_value(config_parser_t *this) {
    return xmlNodeListGetString(this->doc, this->current_node, 1);
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

xmlNode *_go_to_sibling_named(xmlNode *current, const xmlChar *name) {
    if (current == NULL) {
        return NULL;
    }
    for (xmlNode *c = current->next; c != NULL; c = c->next) {
        if (xmlStrEqual(c->name, name)) {
            return c;
        }
    }
    for (xmlNode *c = current->prev; c != NULL; c = c->prev) {
        if (xmlStrEqual(c->name, name)) {
            return c;
        }
    }
    return NULL;
}

bool config_xml_go_to_node_by_path(config_parser_t *this, const xmlChar *path) {
    xmlNode *current_node = this->current_node;
    if (path[0] == '/') {
        current_node = get_config_xml_root(this);
    }
    xmlChar *node_name = _next_node_name(path);
    while (current_node != NULL && node_name != NULL) {
        current_node = _go_to_sibling_named(current_node->children, node_name);
        xmlFree(node_name);
    }
}

xmlChar *_next_node_name(const xmlChar *path) {
    size_t path_length = xmlStrlen(path);
    size_t start = 0;
    size_t end = 0;
    while (path[start] == '/') {
        start++;
        if (start == path_length) {
            break;
        }
    }
    end = start;
    while (path[end + 1] != '/' && path[end + 1]) { end++; }
    end++;
    char *node_name = new_object(sizeof(char) * (end - start + 1));
    if (is_not_created(node_name)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        exit(4);
    }
    strncpy(node_name, path + start, end - start);
    node_name[end - start + 1] = '\0';
    return node_name;
}

bool is_beginning_with_slash(xmlChar *path) {
    return path[0] == '/';
}

int skip_slashes(const xmlChar *path) {
    size_t length = xmlStrlen(path);
    int skip = 0;
    while (path[skip] == '/' && skip < length ) {
        skip++;
    }
    return skip;
}






