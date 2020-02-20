//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_HARMONIC_MOTION_H
#define AQUARIUM_HARMONIC_MOTION_H

#include "object.h"
#include "cartesian_velocity.h"
#include "harmonic_velocity.h"
#include "time_handler.h"

typedef struct {
    long double angle_to_x_axis;
    harmonic_velocity_t * harmonic_v;
    cartesian_velocity_t * cartesian_v;
    time_handler_t * clock;
} harmonic_motion_t;

harmonic_motion_t * new_harmonic_motion(long double amplitude, long double period, long double init_phase,
                                        long double angle_to_x_axis);
harmonic_motion_t * delete_harmonic_motion(harmonic_motion_t * this);

void update_harmonic_motion(harmonic_motion_t * this, long double dtime);

void update_x_harmonic_motion(harmonic_motion_t * this, long double harmonic_velocity_value);

void update_y_harmonic_motion(harmonic_motion_t * this, long double harmonic_velocity_value);

long double get_x_delta_harmonic_velocity(harmonic_motion_t * this);

long double get_y_delta_harmonic_velocity(harmonic_motion_t * this);

void set_harmonic_motion_angle(harmonic_motion_t * this, long double angle);

void update_harmonic_clock(harmonic_motion_t * this, long double dtime);

bool is_possible_to_do_harmonic_motion(harmonic_motion_t * this);

void suspend_harmonic_motion(harmonic_motion_t * this, long double time);

long double count_suspend_time(harmonic_motion_t * this, long double translational_velocity);

#endif //AQUARIUM_HARMONIC_MOTION_H
