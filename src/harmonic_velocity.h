//
// Created by foreverhungry on 17.02.2020.
//

#ifndef AQUARIUM_HARMONIC_VELOCITY_H
#define AQUARIUM_HARMONIC_VELOCITY_H

#include <stdio.h>
#include "object.h"

typedef struct {
    long double amplitude;
    long double period;
    long double phase;
    long double angular_v;
} harmonic_velocity_t;

harmonic_velocity_t * create_harmonic_velocity(long double amplitude, long double period, long double init_phase);

harmonic_velocity_t * destroy_harmonic_velocity(harmonic_velocity_t * this);

void update_harmonic_velocity(harmonic_velocity_t * this, long double dtime);

void update_harmonic_phase(harmonic_velocity_t * this, long double dtime);

long double count_harmonic_angular_velocity(long double period);

long double count_delta_harmonic_velocity(harmonic_velocity_t *this, long double dtime);

#endif //AQUARIUM_HARMONIC_VELOCITY_H
