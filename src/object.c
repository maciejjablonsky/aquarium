#include "object.h"
#include <stddef.h>
#include "memory_handling.h"
#include <stdarg.h>
#include <stdint.h>


void *new_object(size_t size) {
    return malloc(size);
}
void *delete_object(void *object) {
    if (is_created(object)) {
        free(object);
    }
    return NULL;
}

bool is_all_deleted(size_t number_of_objects, ...) {
    va_list objects;
    va_start(objects, number_of_objects);
    for (int i = 0; i < number_of_objects; ++i) {
        if (is_not_deleted(va_arg(objects, void *))) {
            va_end(objects);
            return false;
        }
    }
    va_end(objects);
    return true;
}

bool is_not_created(const void *object) {
    return object == NULL;
}

bool is_created(const void *object) {
    return !is_not_created(object);
}

bool is_deleted(const void *object) {
    return object == NULL;
}

bool is_not_deleted(const void *object) {
    return !is_deleted(object);
}

bool is_all_created(size_t number_of_objects, ...) {
    va_list objects;
    va_start(objects, number_of_objects);
    for (int i = 0; i < number_of_objects; ++i) {
        if (is_not_created(va_arg(objects, void *))) {
            va_end(objects);
            return false;
        }
    }
    va_end(objects);
    return true;
}


