//
// Created by foreverhungry on 18.02.2020.
//

#ifndef AQUARIUM_XML_TREE_H
#define AQUARIUM_XML_TREE_H
#define LIBXML_READER_ENABLED
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>
#include "../object.h"


typedef  struct {
  xmlDocPtr * document;
  xmlNodePtr * current_node;
} xml_tree_t;



FILE * open_xml_file(const char * const filename, const char * const mode);

xml_tree_t * new_xml_tree_t(const char * const filename);



#endif //AQUARIUM_XML_TREE_H
