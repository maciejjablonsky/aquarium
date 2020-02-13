//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_HARMONIC_MOVEMENT_H
#define AQUARIUM_HARMONIC_MOVEMENT_H

#include "object.h"

typedef struct {
    long double amplitude;
    long double period;
    long double init_phase;
    long double angle_to_y_axis;
    long double angular_velocity;
} harmonic_movement_t;

harmonic_movement_t * create_harmonic_movement(long double amplitude, long double period, long double init_phase,
        long double angle_to_y_axis);
harmonic_movement_t * destroy_harmonic_movement(harmonic_movement_t * this);

void set_harmonic_angular_velocity(harmonic_movement_t * this, long double angular_velocity);
long double count_angular_velocity(long double period);
long double get_harmonic_angular_velocity(harmonic_movement_t *this);

long double
get_harmonic_velocity_x(long double amplitude, long double angular_velocity, long double amplitude_change_phase,
                        long double angle);

long double
get_harmonic_velocity_y(long double amplitude, long double angular_velocity, long double amplitude_change_phase,
                        long double angle);

long double count_amplitude_phase(long double angular_velocity, long double time, long double init_phase);
#endif //AQUARIUM_HARMONIC_MOVEMENT_H
