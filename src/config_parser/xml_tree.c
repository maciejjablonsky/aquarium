//
// Created by foreverhungry on 18.02.2020.
//

#include "xml_tree.h"
#include "memory_handling.h"

bool is_not_opened(FILE * file) {
    return file == NULL;
}

FILE *open_xml_file(const char *const filename, const char *const mode) {
    FILE * file = NULL;
    if (filename == NULL) {
        fprintf(stderr, "Filename of xml file is not given.\n");
        return NULL;
    }
    file = fopen(filename, mode);
    if (is_not_opened(file)) {
        fprintf(stderr, "Failed to open %s", filename);
        return NULL;
    }
    return file;
}

xml_tree_t *new_xml_tree_t(const char *const filename) {
    xml_tree_t *this = new_object(sizeof(xml_tree_t));
    if (is_not_created(this)) {
        EXPLICIT_ERROR_MESSAGE("Failed to create xml_tree object.\n");
        return NULL;
    }
    FILE * xml_file = open_xml_file(filename, "r");
    if (is_not_opened(xml_file)) {
        delete_object(this);
        return NULL;
    }


}
