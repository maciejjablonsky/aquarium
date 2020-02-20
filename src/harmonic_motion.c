//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "harmonic_motion.h"
#include "harmonic_velocity.h"
#include "memory_handling.h"
#include <math.h>

static bool is_harmonic_motion_created_properly(harmonic_motion_t *this) {
    if (is_not_created(this->harmonic_v)) {
        NEW_OBJECT_FAILURE("harmonic_velocity_t *");
    }
    if (is_not_created(this->cartesian_v)) {
        NEW_OBJECT_FAILURE("cartesian_velocity_t *");
    }
    if (is_not_created(this->clock)) {
        NEW_OBJECT_FAILURE("time_handler_t");
    }
    return this->harmonic_v && this->cartesian_v && this->clock;
}

harmonic_motion_t *new_harmonic_motion(long double amplitude, long double period, long double init_phase,
                                       long double angle_to_x_axis) {
    harmonic_motion_t *this = new_object(sizeof(harmonic_motion_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }
    this->angle_to_x_axis = angle_to_x_axis;
    this->harmonic_v = new_harmonic_velocity(amplitude, period, init_phase);
    this->cartesian_v = new_cartesian_velocity(0, 0);
    this->clock = new_time_handler();
    if (is_harmonic_motion_created_properly(this)) {
        return this;
    }
    return delete_harmonic_motion(this);
}

harmonic_motion_t *delete_harmonic_motion(harmonic_motion_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    this->harmonic_v = delete_harmonic_velocity(this->harmonic_v);
    this->cartesian_v = delete_cartesian_velocity(this->cartesian_v);
    this->clock = delete_time_handler(this->clock);
    if (is_all_deleted(3, this->harmonic_v, this->cartesian_v, this->clock)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE("* harmonic_motion_t");
        return this;
    }
}

void update_harmonic_motion(harmonic_motion_t *this, long double dtime) {
    update_harmonic_clock(this, dtime);
    long double harmonic_velocity_value = 0;
    if (is_possible_to_do_harmonic_motion(this)) {
        update_harmonic_velocity(this->harmonic_v, dtime);
        harmonic_velocity_value = count_delta_harmonic_velocity(this->harmonic_v, dtime);
    }
    update_x_harmonic_motion(this, harmonic_velocity_value);
    update_y_harmonic_motion(this, harmonic_velocity_value);
}

void update_x_harmonic_motion(harmonic_motion_t *this, long double harmonic_velocity_value) {
    this->cartesian_v->x = harmonic_velocity_value * cosl(this->angle_to_x_axis);
}

void update_y_harmonic_motion(harmonic_motion_t *this, long double harmonic_velocity_value) {
    this->cartesian_v->y = harmonic_velocity_value * sinl(this->angle_to_x_axis);
}

long double get_x_delta_harmonic_velocity(harmonic_motion_t *this) {
    return this->cartesian_v->x;
}

long double get_y_delta_harmonic_velocity(harmonic_motion_t *this) {
    return this->cartesian_v->y;
}

void set_harmonic_motion_angle(harmonic_motion_t *this, long double angle) {
    this->angle_to_x_axis = angle;
}

void update_harmonic_clock(harmonic_motion_t *this, long double dtime) {
    if (this->clock->global > 0) {
        this->clock->global -= dtime;
    }
}

bool is_possible_to_do_harmonic_motion(harmonic_motion_t *this) {
    return this->clock->global <= 0;
}

void suspend_harmonic_motion(harmonic_motion_t *this, long double time) {
    this->clock->global = time;
}

long double count_suspend_time(harmonic_motion_t *this, long double translational_velocity) {
    return fabsl(this->harmonic_v->amplitude * cosl(this->harmonic_v->phase)) / translational_velocity;
}








