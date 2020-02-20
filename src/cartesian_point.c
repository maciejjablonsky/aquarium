//
// Created by foreverhungry on 13.02.2020.
//

#include "cartesian_point.h"
#include "math.h"
#include "object.h"
#include "memory_handling.h"
#include <stdlib.h>
#include <stdio.h>

long double distance_between(const cartesian_point_t *p1, const cartesian_point_t *p2) {
    long double x = p1->x - p2->x;
    long double y = p1->y - p2->y;
    return sqrtl(x*x + y*y);
}

cartesian_point_t *new_cartesian_point(long double x, long double y) {
    cartesian_point_t * this = new_object(sizeof(cartesian_point_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }
    this->x = x;
    this->y = y;
    return this;
}

cartesian_point_t *delete_cartesian_point(cartesian_point_t *this) {
    return delete_object(this);
}
