#include <stdio.h>
#include "cartesian_velocity.h"
#include "object.h"
#include "memory_handling.h"
#include <math.h>
cartesian_velocity_t *new_cartesian_velocity(long double x, long double y) {
    cartesian_velocity_t * this = new_object(sizeof(cartesian_velocity_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        NEW_OBJECT_FAILURE(CARTESIAN_VELOCITY_T_NAME);
        return NULL;
    }
    this->x = x;
    this->y = y;
    return this;
}

cartesian_velocity_t *delete_cartesian_velocity(cartesian_velocity_t *this) {
    return delete_object(this);
}

void set_cartesian_velocity_using_trig(cartesian_velocity_t *this, long double value, long double angle_to_x_axis) {
    this->x = value * cosl(angle_to_x_axis);
    this->y = value * sinl(angle_to_x_axis);
}

long double get_x_cartesian_velocity(cartesian_velocity_t *this) {
    return this->x;
}

long double get_y_cartesian_velocity(cartesian_velocity_t *this) {
    return this->y;
}
