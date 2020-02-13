//
// Created by foreverhungry on 13.02.2020.
//

#include <stdio.h>
#include "../include/harmonic_movement.h"
#include <math.h>

harmonic_movement_t *create_harmonic_movement(long double amplitude, long double period, long double init_phase,
                                              long double angle_to_y_axis) {
    harmonic_movement_t * this = create_object(sizeof(harmonic_movement_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create harmonic movement.\n");
        return NULL;
    }
    this->amplitude = amplitude;
    this->period = period;
    this->init_phase = init_phase;
    this->angle_to_y_axis  = angle_to_y_axis;
    this->angular_velocity = count_angular_velocity(this->period);
    return this;
}

harmonic_movement_t *destroy_harmonic_movement(harmonic_movement_t *this) {
    return destroy_object(this);
}

long double
get_harmonic_velocity_x(long double amplitude, long double angular_velocity, long double amplitude_change_phase,
                        long double angle) {
    return amplitude * angular_velocity * sin(amplitude_change_phase) * sin(angle);
}

long double
get_harmonic_velocity_y(long double amplitude, long double angular_velocity, long double amplitude_change_phase,
                        long double angle) {
    return amplitude * angular_velocity * sin(amplitude_change_phase) * cos(angle);
}

void set_harmonic_angular_velocity(harmonic_movement_t *this, long double angular_velocity) {
    this->angular_velocity = angular_velocity;
}

long double count_angular_velocity(long double period) {
    return (2*M_PI)/period;
}

long double get_harmonic_angular_velocity(harmonic_movement_t *this) {
    return this->angular_velocity;
}

long double count_amplitude_phase(long double angular_velocity, long double time, long double init_phase) {
    return angular_velocity * time + init_phase;
}
