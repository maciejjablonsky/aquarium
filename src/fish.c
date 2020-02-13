//
// Created by maciek on 20.09.19.
//


#include "fish.h"
#include "object.h"
#include "wall.h"
#include "time_handler.h"
#include <math.h>

#define to_radians(degrees) ((degrees) * M_PI / 180)
#define to_degrees(radians) ((radians) * 180 / M_PI)

fish_t *create_fish(size_t max_x, size_t max_y, SDL_Rect
dimensions, long double initial_translational_velocity, long double wave_amplitude, long double wave_period) {
    fish_t *this = create_object(sizeof(fish_t));
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create fish.\n");
        return NULL;
    }

    this->coords = create_cartesian_point((int) (rand() * max_x / RAND_MAX),
                                          (int) (rand() * max_y / RAND_MAX));
    long double angle = to_radians(((long double) rand() / RAND_MAX) * 360);
    this->polar_velocity = create_polar_velocity(initial_translational_velocity, angle, angle);
    this->cartesian_velocity = create_cartesian_velocity(initial_translational_velocity,
                                                         this->polar_velocity->r * cosl(this->polar_velocity->angle),
                                                         this->polar_velocity->r * sinl
                                                                 (this->polar_velocity->angle));
    this->wave = create_harmonic_movement(wave_amplitude, wave_period,
                                          to_radians(((long double) rand() / RAND_MAX) * 360),
                                          this->polar_velocity->angle);
    this->dimensions = dimensions;

    if (is_not_created(this->coords) || is_not_created(this->polar_velocity) || is_not_created
            (this->cartesian_velocity) || is_not_created(this->wave)) {
        return NULL;
    }
    return this;
}


fish_t *destroy_fish(fish_t *this) {
    this->cartesian_velocity = destroy_cartesian_velocity(this->cartesian_velocity);
    this->polar_velocity = destroy_polar_velocity(this->polar_velocity);
    this->coords = destroy_cartesian_point(this->coords);
    this->wave = destroy_harmonic_movement(this->wave);

    if (is_destroyed(this->cartesian_velocity) && is_destroyed(this->polar_velocity) && is_destroyed(this->coords) &&
        is_destroyed(this->wave)) {
        return destroy_object(this);
    }
    return this;
}

//void draw_directions(SDL_Renderer *renderer, fish_t *fish, SDL_Rect *fish_dimensions) {
//    double angle = fish->translational_velocity_angle;
//    int length = 60;
//    int x = length * SDL_cos(angle) + fish->coords.x + fish_dimensions->w / 2;
//    int y = length * SDL_sin(angle) + fish->coords.y + fish_dimensions->h / 2;
//    SDL_RenderDrawLine(renderer, fish->coords.x + fish_dimensions->w / 2, fish->coords.y + fish_dimensions->h / 2, x,
//                       y);
//}

void show_fish(SDL_Renderer *renderer, fish_t *fish, SDL_Texture *fish_texture, SDL_Rect *fish_rectangle) {
    SDL_Point center = {fish_rectangle->w / 2, fish_rectangle->h / 2};
    double angle = to_degrees(atan2l(fish->cartesian_velocity->y, fish->cartesian_velocity->x));

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    double a_angle = fabs(angle);

    if (90 <= a_angle && a_angle < 180) {
        flip = SDL_FLIP_VERTICAL;
    }
    //draw_directions(renderer, fish, fish_rectangle);
    SDL_RenderCopyEx(renderer, fish_texture, NULL, fish_rectangle, angle, &center, flip);
}


void set_fish_velocity_value(fish_t *this, long double value) {
    set_polar_velocity_value(this->polar_velocity, value);
    set_cartesian_velocity_using_trig(this->cartesian_velocity, value, this->polar_velocity->angle);
}

void multiply_fish_velocity(fish_t *this, long double multiplier) {
    set_fish_velocity_value(this, this->polar_velocity->r * multiplier);
}

wall_t which_wall_is_going_to_hit(fish_t *this, cartesian_point_t *aquarium_dimensions) {
    if (is_going_to_hit_top_wall(this)) {
        return TOP;
    } else if (is_going_to_hit_right_wall(this, aquarium_dimensions)) {
        return RIGHT;
    } else if (is_going_to_hit_bottom_wall(this, aquarium_dimensions)) {
        return BOTTOM;
    } else if (is_going_to_hit_left_wall(this)) {
        return LEFT;
    }
    return NONE;
}

bool is_going_to_hit_top_wall(fish_t *this) {
    return this->coords->y + get_velocity_y_of(this) <= 0;
}

bool is_going_to_hit_right_wall(fish_t *this, cartesian_point_t *aquarium_dimensions) {
    return this->coords->x + this->dimensions.w + get_velocity_x_of(this) >= aquarium_dimensions->x;
}

bool is_going_to_hit_bottom_wall(fish_t *this, cartesian_point_t *aquarium_dimensions) {
    return this->coords->y + this->dimensions.h + get_velocity_y_of(this) >= aquarium_dimensions->y;
}

bool is_going_to_hit_left_wall(fish_t *this) {
    return this->coords->x + get_velocity_x_of(this) <= 0;
}

void update_velocity_x_of(fish_t *this, time_handler_t *clock) {
    this->cartesian_velocity->x = get_translational_velocity_x_of(this, clock->dtime) + get_harmonic_velocity_x_of(this,
                                                                                                                   clock->global);
}


long double get_velocity_y_of(fish_t *this) {
    return this->cartesian_velocity->y;
}

long double get_velocity_x_of(fish_t *this) {
    return this->cartesian_velocity->x;
}

void update_velocity_y_of(fish_t *this, time_handler_t *clock) {
    this->cartesian_velocity->y = get_translational_velocity_y_of(this, clock->dtime) + get_harmonic_velocity_y_of
            (this, clock->global);
}

void update_velocity_of(fish_t *this, time_handler_t *clock) {
    update_velocity_x_of(this, clock);
    update_velocity_y_of(this, clock);
}

long double get_translational_velocity_x_of(fish_t *this, long double dtime) {
    return this->polar_velocity->r * cos(this->polar_velocity->angle) * dtime;
}

long double get_translational_velocity_y_of(fish_t *this, long double dtime) {
    return this->polar_velocity->r * sin(this->polar_velocity->angle) * dtime;
}

long double get_harmonic_velocity_x_of(fish_t *this, long double global_time) {
    return get_harmonic_velocity_x(this->wave->amplitude, get_harmonic_angular_velocity(this->wave),
                                   this->wave->angular_velocity * global_time + this->wave->init_phase,
                                   -(this->wave->angle_to_y_axis));
}

long double get_harmonic_velocity_y_of(fish_t *this, long double global_time) {
    return get_harmonic_velocity_y(this->wave->amplitude, get_harmonic_angular_velocity(this->wave),
                                   count_amplitude_phase(this->wave->angular_velocity, global_time,
                                                         this->wave->init_phase),
                                   -(this->wave->angle_to_y_axis));
}

bool is_going_to_hit_wall(wall_t wall) {
    return wall != NONE;
}

void dodge_wall(fish_t *this, wall_t wall_to_hit) {
    if (wall_to_hit == TOP || wall_to_hit == BOTTOM) {
        dodge_horizontal_wall(this);
    } else {
        dodge_vertical_wall(this);
    }
}

void dodge_horizontal_wall(fish_t *this) {
    long double x = this->polar_velocity->r * cosl(this->polar_velocity->angle);
    long double y = -(this->polar_velocity->r * sinl(this->polar_velocity->angle));
    this->polar_velocity->angle = atan2l(y, x);
}

void dodge_vertical_wall(fish_t *this) {
    long double x = -(this->polar_velocity->r * cosl(this->polar_velocity->angle));
    long double y = this->polar_velocity->r * sinl(this->polar_velocity->angle);
    this->polar_velocity->angle = atan2l(y, x);
}

void update_fish_coordinates(fish_t *this) {
    this->coords->x += get_velocity_x_of(this);
    this->coords->y += get_velocity_y_of(this);
}

void update_fish_dimensions(fish_t *this) {
    this->dimensions.x = this->coords->x;
    this->dimensions.y = this->coords->y;
}


