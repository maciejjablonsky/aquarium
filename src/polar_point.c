//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "polar_point.h"

polar_point_t *new_polar_point(long double r, long double angle, long double init_phase) {
    polar_point_t * this = new_object(sizeof(polar_point_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create polar point.\n");
        return NULL;
    }
    this->r = r;
    this->angle = angle;
    this->init_phase = init_phase;
    return this;
}

polar_point_t *delete_polar_point(polar_point_t *this) {
    return delete_object(this);
}
