//
// Created by foreverhungry on 18.02.2020.
//

#ifndef AQUARIUM_XML_TREE_H
#define AQUARIUM_XML_TREE_H
#include <libxml2/libxml/tree.h>


typedef  struct {
  xmlDocPtr * document;
  xmlNodePtr * current_node;
} xml_tree_t;



#endif //AQUARIUM_XML_TREE_H
