//
// Created by foreverhungry on 17.02.2020.
//

#include <math.h>
#include "harmonic_velocity.h"

harmonic_velocity_t *create_harmonic_velocity(long double amplitude, long double period, long double init_phase) {
    harmonic_velocity_t * this = create_object(sizeof(harmonic_velocity_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create harmonic velocity object.\n");
        return NULL;
    }
    this->amplitude = amplitude;
    this->period = period;
    this->phase = init_phase;
    this->angular_v = count_harmonic_angular_velocity(period);
    return this;
}

harmonic_velocity_t *destroy_harmonic_velocity(harmonic_velocity_t *this) {
    return destroy_object(this);
}

void update_harmonic_velocity(harmonic_velocity_t *this, long double dtime) {
    update_harmonic_phase(this, dtime);
}

void update_harmonic_phase(harmonic_velocity_t *this, long double dtime) {
    this->phase += this->angular_v * dtime;
}

long double count_harmonic_angular_velocity(long double period) {
    return (2*M_PI)/period;
}

long double count_delta_harmonic_velocity(harmonic_velocity_t *this, long double dtime) {
    return this->amplitude * this->angular_v * cosl(this->phase) * dtime;
}




