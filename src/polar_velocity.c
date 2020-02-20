//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "polar_velocity.h"
#include "object.h"

polar_velocity_t *new_polar_velocity(long double r, long double angle, long double init_phase) {
    polar_velocity_t * this = new_object(sizeof(polar_velocity_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create polar velocity.\n");
        return NULL;
    }
    this->r = r;
    this->angle = angle;
    this->init_phase = init_phase;
    return this;
}

polar_velocity_t *delete_polar_velocity(polar_velocity_t *this) {
    return delete_object(this);
}

void set_polar_velocity_value(polar_velocity_t *this, long double value) {
    this->r = value;
}
