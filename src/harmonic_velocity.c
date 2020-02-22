#include <math.h>
#include "harmonic_velocity.h"
#include "memory_handling.h"

typedef enum {
    TRANSLATIONAL_MOTION_NO_MEMORY
} harmonic_velocity_error_code_t;

static harmonic_velocity_t * delete_error_harmonic_velocity_t(harmonic_velocity_t * this,
        harmonic_velocity_error_code_t error_code);

harmonic_velocity_t *new_harmonic_velocity(long double amplitude, long double period, long double init_phase) {
    harmonic_velocity_t * this = new_object(sizeof(harmonic_velocity_t));
    if (is_not_created(this)) {
        return delete_error_harmonic_velocity_t(this, TRANSLATIONAL_MOTION_NO_MEMORY);
    }
    this->amplitude = amplitude;
    this->period = period;
    this->phase = init_phase;
    this->angular_v = count_harmonic_angular_velocity(period);
    return this;
}

static harmonic_velocity_t * delete_error_harmonic_velocity_t(harmonic_velocity_t * this,
                                                              harmonic_velocity_error_code_t error_code) {
    switch(error_code) {
        case TRANSLATIONAL_MOTION_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        default:
            break;
    }
    return delete_harmonic_velocity(this);
}

harmonic_velocity_t *delete_harmonic_velocity(harmonic_velocity_t *this) {
    return delete_object(this);
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




