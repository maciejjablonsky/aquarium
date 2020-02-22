#include <stdio.h>
#include "translational_motion.h"
#include "object.h"
#include "memory_handling.h"
#include <math.h>

typedef enum {
    TRANSLATIONAL_MOTION_NO_MEMORY,
    TRANSLATIONAL_MOTION_DELTA_MOTION_FAIL,
    TRANSLATIONAL_MOTION_POLAR_VELOCITY_FAIL,
    TRANSLATIONAL_MOTION_CARTESIAN_VELOCITY_FAIL
} translational_motion_error_code_t;

static translational_motion_t *delete_error_translational_motion(translational_motion_t *this,
                                                                 translational_motion_error_code_t error_code);

static translational_motion_error_code_t get_translational_motion_error_code(translational_motion_t *this);

translational_motion_t *
new_translational_motion(long double x_0, long double y_0, long double velocity_value, long double angle,
                         long double init_phase) {
    translational_motion_t *this = new_object(sizeof(translational_motion_t));
    if (is_not_created(this)) {
        return delete_error_translational_motion(this, TRANSLATIONAL_MOTION_NO_MEMORY);
    }
    this->delta_motion = new_cartesian_point(x_0, y_0);
    this->polar_v = new_polar_velocity(velocity_value, angle, init_phase);
    this->cartesian_v = new_cartesian_velocity(
            velocity_value * cosl(angle),
            velocity_value * sinl(angle));
    if (IS_ALL_CREATED(this->polar_v, this->cartesian_v, this->delta_motion)) {
        return this;
    } else {
        return delete_error_translational_motion(this, get_translational_motion_error_code(this));
    }
}

static translational_motion_error_code_t get_translational_motion_error_code(translational_motion_t *this) {
    if (is_not_created(this)) return TRANSLATIONAL_MOTION_NO_MEMORY;
    else if (is_not_created(this->delta_motion)) return TRANSLATIONAL_MOTION_DELTA_MOTION_FAIL;
    else if (is_not_created(this->cartesian_v)) return TRANSLATIONAL_MOTION_CARTESIAN_VELOCITY_FAIL;
    else if (is_not_created(this->polar_v)) return TRANSLATIONAL_MOTION_POLAR_VELOCITY_FAIL;
    else return -1;
}

static translational_motion_t *delete_error_translational_motion(translational_motion_t *this,
                                                                 translational_motion_error_code_t error_code) {
    switch (error_code) {
        case TRANSLATIONAL_MOTION_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case TRANSLATIONAL_MOTION_DELTA_MOTION_FAIL:
            NEW_OBJECT_FAILURE(CARTESIAN_POINT_T_NAME);
            break;
        case TRANSLATIONAL_MOTION_POLAR_VELOCITY_FAIL:
            NEW_OBJECT_FAILURE(POLAR_VELOCITY_T_NAME);
            break;
        case TRANSLATIONAL_MOTION_CARTESIAN_VELOCITY_FAIL:
            NEW_OBJECT_FAILURE(CARTESIAN_VELOCITY_T_NAME);
            break;
        default:
            break;
    }
    return NULL;
}

translational_motion_t *delete_translational_motion(translational_motion_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    this->delta_motion = delete_cartesian_point(this->delta_motion);
    this->polar_v = delete_polar_velocity(this->polar_v);
    this->cartesian_v = delete_cartesian_velocity(this->cartesian_v);
    if (IS_ALL_DELETED(this->delta_motion, this->polar_v, this->cartesian_v)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE(TRANSLATIONAL_MOTION_T_NAME);
        exit(2);
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

