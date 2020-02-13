//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_OBJECT_H
#define AQUARIUM_OBJECT_H

#include <stdbool.h>
#include <stddef.h>

void * create_object(size_t size);
void * destroy_object(void * object);
bool is_not_created(void *object);

bool is_created(void *object);


bool is_destroyed(void * object);

bool is_not_destroyed(void * object);



#endif //AQUARIUM_OBJECT_H
