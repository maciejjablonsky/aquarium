#include <stdio.h>
#include "polar_velocity.h"
#include "object.h"
#include "memory_handling.h"

typedef enum {
    TRANSLATIONAL_MOTION_NO_MEMORY
} polar_velocity_error_code_t;

static polar_velocity_t * delete_error_polar_velocity(polar_velocity_t * this, polar_velocity_error_code_t error_code);

polar_velocity_t *new_polar_velocity(long double r, long double angle, long double init_phase) {
    polar_velocity_t * this = new_object(sizeof(polar_velocity_t));
    if (is_not_created(this)) {
        return delete_error_polar_velocity(this, TRANSLATIONAL_MOTION_NO_MEMORY);
    }
    this->r = r;
    this->angle = angle;
    this->init_phase = init_phase;
    return this;
}

polar_velocity_t *delete_polar_velocity(polar_velocity_t *this) {
    return delete_object(this);
}

static polar_velocity_t * delete_error_polar_velocity(polar_velocity_t * this, polar_velocity_error_code_t error_code) {
    switch(error_code) {
        case TRANSLATIONAL_MOTION_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        default:
            break;
    }
    return delete_polar_velocity(this);
}

void set_polar_velocity_value(polar_velocity_t *this, long double value) {
    this->r = value;
}
