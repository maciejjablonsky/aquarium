#include <stdio.h>
#include "polar_point.h"
#include "memory_handling.h"

typedef enum {
    TRANSLATIONAL_MOTION_NO_MEMORY
} polar_point_error_code_t;

static polar_point_t *delete_error_polar_point_t(polar_point_t *this, polar_point_error_code_t error_code);

polar_point_t *new_polar_point(long double r, long double angle, long double init_phase) {
    polar_point_t *this = new_object(sizeof(polar_point_t));
    if (is_not_created(this)) {
        return delete_error_polar_point_t(this, TRANSLATIONAL_MOTION_NO_MEMORY);
    }
    this->r = r;
    this->angle = angle;
    this->init_phase = init_phase;
    return this;
}

static polar_point_t *delete_error_polar_point_t(polar_point_t *this, polar_point_error_code_t error_code) {
    switch (error_code) {
        case TRANSLATIONAL_MOTION_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        default:
            break;
    }
    return delete_polar_point(this);
}

polar_point_t *delete_polar_point(polar_point_t *this) {
    return delete_object(this);
}
