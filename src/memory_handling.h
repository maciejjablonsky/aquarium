#ifndef AQUARIUM_MEMORY_HANDLING_H
#define AQUARIUM_MEMORY_HANDLING_H

#include <stdlib.h>

#ifdef DEBUG

#include <stdio.h>

#define MEMORY_NOT_ALLOCATED_MESSAGE() fprintf(stderr, "[FILE: %s][LINE: %d] null pointer or memory allocation rejection.\n", __FILE__, __LINE__)
#else
#define MEMORY_NOT_ALLOCATED_MESSAGE()
#endif

#include <stdbool.h>
#include <stddef.h>

#endif //AQUARIUM_MEMORY_HANDLING_H
