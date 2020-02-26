#ifndef AQUARIUM_TRANSLATIONAL_MOTION_H
#define AQUARIUM_TRANSLATIONAL_MOTION_H

#include "polar_velocity.h"
#include "cartesian_velocity.h"
#include "cartesian_point.h"

typedef struct {
    polar_velocity_t * polar_v;
    cartesian_velocity_t * cartesian_v;
    cartesian_point_t * delta_motion;
} translational_motion_t;
#define TRANSLATIONAL_MOTION_T_NAME "translational_motion_t"

translational_motion_t *
new_translational_motion(long double x_0, long double y_0, long double velocity_value, long double angle,
                         long double init_phase);
translational_motion_t * delete_translational_motion(translational_motion_t * this);

void update_translational_motion(translational_motion_t *this, long double dtime);

void update_x_translational_reposition(translational_motion_t *this, long double dtime);

void update_y_translational_reposition(translational_motion_t *this, long double dtime);

long double count_x_translational_delta_velocity(translational_motion_t *this, long double dtime);

long double count_y_translational_delta_velocity(translational_motion_t * this, long double dtime);

void set_translational_motion_velocity(translational_motion_t * this, long double value);

long double get_translational_velocity_value(translational_motion_t * this);

long double get_x_translational_velocity(translational_motion_t * this);

long double get_y_translational_velocity(translational_motion_t * this);

void set_translational_motion_angle(translational_motion_t * this, long double angle);

long double get_x_delta_translational_velocity(translational_motion_t * this);

long double get_y_delta_translational_velocity(translational_motion_t * this);

#endif //AQUARIUM_TRANSLATIONAL_MOTION_H
