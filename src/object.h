//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_OBJECT_H
#define AQUARIUM_OBJECT_H

#include <stdbool.h>
#include <stddef.h>

#include <stdio.h>

#define EXPLICIT_ERROR_MESSAGE(...) fprintf(stderr, "%s\n", __VA_ARGS__)

#if defined(DEBUG)
#include <stdarg.h>

#define NEW_OBJECT_FAILURE(TYPE) \
        fprintf(stderr, "[FILE: %s][LINE: %d] Failed to create object ("#TYPE").\n", __FILE__, __LINE__)
#define DELETE_OBJECT_FAILURE(TYPE) \
        fprintf(stderr, "[FILE: %s][LINE: %d] Cannot delete object ("#TYPE").\n", __FILE__, __LINE__)
#define IMPLICIT_ERROR_MESSAGE(...) fprintf(stderr, "[FILE: %s][LINE: %d] %s.\n", __FILE__, __LINE__, __VA_ARGS__)
#else
#define IMPLICIT_ERROR_MESSAGE(...)
#define NEW_OBJECT_FAILURE(TYPE)
#define DELETE_OBJECT_FAILURE(TYPE)
#endif

void *new_object(size_t size);

void *delete_object(void *object);

bool is_not_created(const void *object);

bool is_created(const void *object);


bool is_deleted(const void *object);

bool is_not_deleted(const void *object);

bool is_all_deleted(size_t number_of_objects, ...);


#endif //AQUARIUM_OBJECT_H
