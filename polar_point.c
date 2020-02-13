//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "polar_point.h"

polar_point_t *create_polar_point(long double r, long double angle, long double init_phase) {
    polar_point_t * this = create_object(sizeof(polar_point_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create polar point.\n");
        return NULL;
    }
    this->r = r;
    this->angle = angle;
    this->init_phase = init_phase;
    return this;
}

polar_point_t *destroy_polar_point(polar_point_t *this) {
    return destroy_object(this);
}
