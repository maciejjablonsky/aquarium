#include "cartesian_point.h"
#include "math.h"
#include "object.h"
#include "memory_handling.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    TRANSLATIONAL_MOTION_NO_MEMORY
} cartesian_point_error_code_t;

static cartesian_point_t * delete_failed_cartesian_point(cartesian_point_t * this, cartesian_point_error_code_t error_code);

long double distance_between(const cartesian_point_t *p1, const cartesian_point_t *p2) {
    long double x = p1->x - p2->x;
    long double y = p1->y - p2->y;
    return sqrtl(x*x + y*y);
}

cartesian_point_t *new_cartesian_point(long double x, long double y) {
    cartesian_point_t * this = new_object(sizeof(cartesian_point_t));
    if (is_not_created(this)) {
        return delete_failed_cartesian_point(this, TRANSLATIONAL_MOTION_NO_MEMORY);
    }
    this->x = x;
    this->y = y;
    return this;
}

cartesian_point_t *delete_cartesian_point(cartesian_point_t *this) {
    return delete_object(this);
}

static cartesian_point_t * delete_failed_cartesian_point(cartesian_point_t * this, cartesian_point_error_code_t error_code) {
    switch(error_code) {
        case TRANSLATIONAL_MOTION_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        default:
            break;
    }
    return delete_cartesian_point(this);
}
