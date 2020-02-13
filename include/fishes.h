//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_FISHES_H
#define AQUARIUM_FISHES_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL_rect.h>
#include "include/dl_list.h"
#include "time_handler.h"
#include "include/cartesian_point.h"

typedef struct {
    SDL_Rect fish_dimensions;
    double fish_scale;
    long double initial_translational_velocity;
    long double amplitude;
    long double wave_movement_period;
    int max_x;
    int max_y;

} fishes_initial_data_t;

typedef dl_list_t fishes_t;

fishes_t * create_fishes(fishes_initial_data_t * fish_initial_data, size_t amount_of_fishes);

fishes_t * destroy_fishes(fishes_t * this);

void multiply_fishes_velocity(fishes_t * this, long double multiplier);

void move_each_fish_in_aquarium(fishes_t *fishes, time_handler_t *clock, cartesian_point_t aquarium_dimensions);


#endif //AQUARIUM_FISHES_H
