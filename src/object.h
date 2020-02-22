#ifndef AQUARIUM_OBJECT_H
#define AQUARIUM_OBJECT_H

#include <stdbool.h>
#include <stddef.h>

#include <stdio.h>
#include <limits.h>

#define TO_STR(TEXT) ("#TEXT")
#define EXPLICIT_ERROR_MESSAGE(...) fprintf(stderr, "%s\n", __VA_ARGS__)

#define NUM_POINTERS(...) (sizeof((long[]){__VA_ARGS__})/ sizeof(long))

#define IS_ALL_CREATED(...) (is_all_created(NUM_POINTERS(__VA_ARGS__), __VA_ARGS__))

#define IS_ALL_DELETED(...) (is_all_deleted(NUM_POINTERS(__VA_ARGS__), __VA_ARGS__))
#if defined(DEBUG)
#include <stdarg.h>

#define NEW_OBJECT_FAILURE(TYPE_NAME) \
        fprintf(stderr, "[FILE: %s][LINE: %d] Failed to create object (%s).\n", __FILE__, __LINE__, (TYPE_NAME))
#define DELETE_OBJECT_FAILURE(TYPE_NAME) \
        fprintf(stderr, "[FILE: %s][LINE: %d] Cannot delete object (%s).\n", __FILE__, __LINE__, (TYPE_NAME))
#define IMPLICIT_ERROR_MESSAGE(...) do {\
fprintf(stderr, "[FILE: %s][LINE: %d] ", __FILE__, __LINE__);\
fprintf(stderr, __VA_ARGS__);\
fputc('\n', stderr); } while(0)
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

bool is_all_created(size_t number_of_objects, ...);

bool is_all_deleted(size_t number_of_objects, ...);


#endif //AQUARIUM_OBJECT_H
