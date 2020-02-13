//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "../include/cartesian_velocity.h"
#include "../include/object.h"
#include <math.h>
cartesian_velocity_t *create_cartesian_velocity(long double r, long double x, long double y) {
    cartesian_velocity_t * this = create_object(sizeof(cartesian_velocity_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create cartesian velocity.\n");
        return NULL;
    }
    this->r = r;
    this->x = x;
    this->y = y;
    return this;
}

cartesian_velocity_t *destroy_cartesian_velocity(cartesian_velocity_t *this) {
    return destroy_object(this);
}

void set_cartesian_velocity_using_trig(cartesian_velocity_t *this, long double value, long double angle_to_x_axis) {
    this->r = value;
    this->x = value * cosl(angle_to_x_axis);
    this->y = value * sinl(angle_to_x_axis);
}
