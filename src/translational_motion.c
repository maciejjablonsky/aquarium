//
// Created by foreverhungry on 17.02.2020.
//

#include <stdio.h>
#include "translational_motion.h"
#include "object.h"
#include <math.h>

translational_motion_t *
create_translational_motion(long double x_0, long double y_0, long double velocity_value, long double angle,
                            long double init_phase) {
    translational_motion_t *this = create_object(sizeof(translational_motion_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create translational motion object.\n");
        return NULL;
    }
    this->delta_motion = create_cartesian_point(x_0, y_0);
    this->polar_v = create_polar_velocity(velocity_value, angle, init_phase);
    this->cartesian_v = create_cartesian_velocity(
            velocity_value * cosl(this->polar_v->angle),
            velocity_value * sinl
                    (this->polar_v->angle));
    if (is_not_created(this->delta_motion) || is_not_created(this->polar_v) || is_not_created(this->cartesian_v)) {
        return NULL;
    }
    return this;
}

translational_motion_t *destroy_translational_motion(translational_motion_t *this) {
    this->delta_motion = destroy_cartesian_point(this->delta_motion);
    this->polar_v = destroy_polar_velocity(this->polar_v);
    this->cartesian_v = destroy_cartesian_velocity(this->cartesian_v);
    if (is_destroyed(this->delta_motion) && is_destroyed(this->polar_v) && is_destroyed(this->cartesian_v)) {
        return NULL;
    }
    return this;
}

void update_translational_motion(translational_motion_t *this, long double dtime) {
    update_x_translational_reposition(this, dtime);
    update_y_translational_reposition(this, dtime);
}

void update_x_translational_reposition(translational_motion_t *this, long double dtime) {
    this->delta_motion->x = count_x_translational_delta_velocity(this, dtime);
}

void update_y_translational_reposition(translational_motion_t *this, long double dtime) {
    this->delta_motion->y = count_y_translational_delta_velocity(this, dtime);
}

long double count_x_translational_delta_velocity(translational_motion_t *this, long double dtime) {
    return get_x_cartesian_velocity(this->cartesian_v) * dtime;
}

long double count_y_translational_delta_velocity(translational_motion_t *this, long double dtime) {
    return get_y_cartesian_velocity(this->cartesian_v) * dtime;
}

void set_translational_motion_velocity(translational_motion_t *this, long double value) {
    set_polar_velocity_value(this->polar_v, value);
    set_cartesian_velocity_using_trig(this->cartesian_v, value, this->polar_v->angle);
}

long double get_translational_velocity_value(translational_motion_t *this) {
    return this->polar_v->r;
}

long double get_x_translational_velocity(translational_motion_t *this) {
    return this->cartesian_v->x;
}

long double get_y_translational_velocity(translational_motion_t *this) {
    return this->cartesian_v->y;
}

void set_translational_motion_angle(translational_motion_t *this, long double angle) {
    this->polar_v->angle = angle;
    set_cartesian_velocity_using_trig(this->cartesian_v, this->polar_v->r, angle);
}

long double get_x_delta_translational_velocity(translational_motion_t *this) {
    return this->delta_motion->x;
}

long double get_y_delta_translational_velocity(translational_motion_t *this) {
    return this->delta_motion->y;
}

