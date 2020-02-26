#ifndef AQUARIUM_POLAR_VELOCITY_H
#define AQUARIUM_POLAR_VELOCITY_H

typedef struct {
    long double r;
    long double angle;
    long double init_phase;
} polar_velocity_t;
#define POLAR_VELOCITY_T_NAME "polar_velocity_t"

polar_velocity_t * new_polar_velocity(long double r, long double angle, long double init_phase);
polar_velocity_t * delete_polar_velocity(polar_velocity_t * this);
void set_polar_velocity_value(polar_velocity_t * this, long double value);
#endif //AQUARIUM_POLAR_VELOCITY_H
