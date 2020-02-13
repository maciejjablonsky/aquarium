//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_CARTESIAN_VELOCITY_H
#define AQUARIUM_CARTESIAN_VELOCITY_H

typedef struct
{
    long double x;
    long double y;
    long double r;
} cartesian_velocity_t;

cartesian_velocity_t * create_cartesian_velocity(long double r, long double x, long double y);
cartesian_velocity_t * destroy_cartesian_velocity(cartesian_velocity_t * this);
void set_cartesian_velocity_using_trig(cartesian_velocity_t * this, long double value, long double angle_to_x_axis);

#endif //AQUARIUM_CARTESIAN_VELOCITY_H
