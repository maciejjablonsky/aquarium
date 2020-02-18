//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_POLAR_VELOCITY_H
#define AQUARIUM_POLAR_VELOCITY_H

typedef struct {
    long double r;
    long double angle;
    long double init_phase;
} polar_velocity_t;

polar_velocity_t * create_polar_velocity(long double r, long double angle, long double init_phase);
polar_velocity_t * destroy_polar_velocity(polar_velocity_t * this);
void set_polar_velocity_value(polar_velocity_t * this, long double value);
#endif //AQUARIUM_POLAR_VELOCITY_H
