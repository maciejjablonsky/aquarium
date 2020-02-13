//
// Created by maciek on 20.09.19.
//

#ifndef AQUARIUM_FISH_H
#define AQUARIUM_FISH_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "cartesian_point.h"
#include "cartesian_velocity.h"
#include "polar_velocity.h"
#include "harmonic_movement.h"
#include "time_handler.h"
#include "wall.h"

typedef struct fish {
    polar_velocity_t *polar_velocity;
    cartesian_velocity_t *cartesian_velocity;
    cartesian_point_t *coords;
    SDL_Rect dimensions;
    harmonic_movement_t *wave;
} fish_t;

fish_t *create_fish(size_t max_x, size_t max_y, SDL_Rect
dimensions, long double initial_translational_velocity, long double wave_amplitude, long double wave_period);

fish_t *destroy_fish(fish_t *fish);

void show_fish(SDL_Renderer *renderer, fish_t *fish, SDL_Texture *fish_texture, SDL_Rect *fish_rectangle);

void set_fish_velocity_value(fish_t * this, long double value);

void multiply_fish_velocity(fish_t * this, long double multiplier);

wall_t which_wall_is_going_to_hit(fish_t *this, cartesian_point_t *aquarium_dimensions);

bool is_going_to_hit_top_wall(fish_t *this);

bool is_going_to_hit_right_wall(fish_t *this, cartesian_point_t *aquarium_dimensions);

bool is_going_to_hit_bottom_wall(fish_t *this, cartesian_point_t *aquarium_dimensions);

bool is_going_to_hit_left_wall(fish_t *this);

long double get_velocity_x_of(fish_t * this);
void update_velocity_x_of(fish_t * this, time_handler_t * clock);

long double get_velocity_y_of(fish_t *this);
void update_velocity_y_of(fish_t * this, time_handler_t * clock);

void update_velocity_of(fish_t * this, time_handler_t * clock);

long double get_translational_velocity_x_of(fish_t * this, long double dtime);
long double get_translational_velocity_y_of(fish_t * this, long double dtime);

long double get_harmonic_velocity_x_of(fish_t * this, long double global_time);
long double get_harmonic_velocity_y_of(fish_t * this, long double global_time);

bool is_going_to_hit_wall(wall_t wall);

void dodge_wall(fish_t * this, wall_t wall_to_hit);
void dodge_horizontal_wall(fish_t * this);
void dodge_vertical_wall(fish_t * this);

void update_fish_coordinates(fish_t *this);
void update_fish_dimensions(fish_t * this);

#endif //AQUARIUM_FISH_H
