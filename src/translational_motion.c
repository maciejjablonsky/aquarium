//
// Created by foreverhungry on 17.02.2020.
//

#include <stdio.h>
#include "translational_motion.h"
#include "object.h"
#include "memory_handling.h"
#include <math.h>

static bool is_translational_motion_created_properly(translational_motion_t *this) {
    if (is_not_created(this->polar_v)) {
        NEW_OBJECT_FAILURE(*polar_velocity_t);
    }
    if (is_not_created(this->cartesian_v)) {
        NEW_OBJECT_FAILURE(*cartesian_velocity_t);
    }
    if (is_not_created(this->delta_motion)) {
        NEW_OBJECT_FAILURE(*cartesian_point);
    }
    return this->polar_v && this->cartesian_v && this->delta_motion;
}

translational_motion_t *
new_translational_motion(long double x_0, long double y_0, long double velocity_value, long double angle,
                         long double init_phase) {
    translational_motion_t *this = new_object(sizeof(translational_motion_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }
    this->delta_motion = new_cartesian_point(x_0, y_0);
    this->polar_v = new_polar_velocity(velocity_value, angle, init_phase);
    this->cartesian_v = new_cartesian_velocity(
            velocity_value * cosl(angle),
            velocity_value * sinl(angle));
    if (is_translational_motion_created_properly(this)) {
        return this;
    } else {
        return delete_translational_motion(this);
    }
}

translational_motion_t *delete_translational_motion(translational_motion_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    this->delta_motion = delete_cartesian_point(this->delta_motion);
    this->polar_v = delete_polar_velocity(this->polar_v);
    this->cartesian_v = delete_cartesian_velocity(this->cartesian_v);
    if (is_all_deleted(3, this->delta_motion, this->polar_v, this->cartesian_v)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE("* translational_motion_t");
        return this;
    }
}

void update_translational_motion(translational_motion_t *this, long double dtime) {
    update_x_translational_reposition(this, dtime);
    update_y_translational_reposition(this, dtime);
}

void update_x_translational_reposition(translational_motion_t *this, long double dtime) {
    this->delta_motion->x = count_x_translational_delta_velocity(this, dtime);
}

void update_y_translational_reposition(translational_motion_t *this, long double dtime) {
    this->delta_motion->y = count_y_translational_delta_velocity(this, dtime);
}

long double count_x_translational_delta_velocity(translational_motion_t *this, long double dtime) {
    return get_x_cartesian_velocity(this->cartesian_v) * dtime;
}

long double count_y_translational_delta_velocity(translational_motion_t *this, long double dtime) {
    return get_y_cartesian_velocity(this->cartesian_v) * dtime;
}

void set_translational_motion_velocity(translational_motion_t *this, long double value) {
    set_polar_velocity_value(this->polar_v, value);
    set_cartesian_velocity_using_trig(this->cartesian_v, value, this->polar_v->angle);
}

long double get_translational_velocity_value(translational_motion_t *this) {
    return this->polar_v->r;
}

long double get_x_translational_velocity(translational_motion_t *this) {
    return this->cartesian_v->x;
}

long double get_y_translational_velocity(translational_motion_t *this) {
    return this->cartesian_v->y;
}

void set_translational_motion_angle(translational_motion_t *this, long double angle) {
    this->polar_v->angle = angle;
    set_cartesian_velocity_using_trig(this->cartesian_v, this->polar_v->r, angle);
}

long double get_x_delta_translational_velocity(translational_motion_t *this) {
    return this->delta_motion->x;
}

long double get_y_delta_translational_velocity(translational_motion_t *this) {
    return this->delta_motion->y;
}
