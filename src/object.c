//
// Created by foreverhungry on 13.02.2020.
//

#include "object.h"
#include <stddef.h>
#include <stdlib.h>


void *create_object(size_t size) {
    return malloc(size);
}
void *destroy_object(void *object) {
    if (is_created(object)) {
        free(object);
    }
    return NULL;
}

bool is_not_created(void *object) {
    return object == NULL;
}

bool is_created(void *object) {
    return !is_not_created(object);
}

bool is_destroyed(void *object) {
    return object == NULL;
}

bool is_not_destroyed(void *object) {
    return !is_destroyed(object);
}


