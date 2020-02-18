//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "harmonic_motion.h"
#include "harmonic_velocity.h"
#include <math.h>

harmonic_motion_t *create_harmonic_motion(long double amplitude, long double period, long double init_phase,
                                          long double angle_to_x_axis) {
    harmonic_motion_t * this = create_object(sizeof(harmonic_motion_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create harmonic movement.\n");
        return NULL;
    }
    this->angle_to_x_axis = angle_to_x_axis;
    this->harmonic_v = create_harmonic_velocity(amplitude, period, init_phase);
    this->cartesian_v = create_cartesian_velocity(0,0);
    this->clock = create_time_handler();
    if (is_not_created(this->harmonic_v) || is_not_created(this->cartesian_v) || is_not_created(this->clock)) {
        return NULL;
    }
    return this;
}

harmonic_motion_t *destroy_harmonic_motion(harmonic_motion_t *this) {
    this->harmonic_v = destroy_harmonic_velocity(this->harmonic_v);
    this->cartesian_v = destroy_cartesian_velocity(this->cartesian_v);
    this->clock = destroy_time_handler(this->clock);
    if (is_destroyed(this->harmonic_v) && is_destroyed(this->cartesian_v) && is_destroyed(this->clock)) {
        return destroy_object(this);
    }
    return this;
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








