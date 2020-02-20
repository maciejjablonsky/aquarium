//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_POLAR_POINT_H
#define AQUARIUM_POLAR_POINT_H

#include "object.h"

typedef struct {
    long double r;
    long double angle;
    long double init_phase;
} polar_point_t;

polar_point_t * new_polar_point(long double r, long double angle, long double init_phase);
polar_point_t * delete_polar_point(polar_point_t * this);

#endif //AQUARIUM_POLAR_POINT_H
