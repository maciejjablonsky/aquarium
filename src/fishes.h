#ifndef AQUARIUM_FISHES_H
#define AQUARIUM_FISHES_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL_rect.h>
#include "dl_list.h"
#include "time_handler.h"
#include "cartesian_point.h"
#include <libxml/tree.h>
#include "config_parser/config_parser.h"

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
#define FISHES_T_NAME "fishes_t"

fishes_t * new_fishes(fishes_initial_data_t * fish_initial_data, size_t amount_of_fishes);

fishes_t * delete_fishes(fishes_t * this);

void multiply_fishes_velocity(fishes_t * this, long double multiplier);

void move_each_fish_in_aquarium(fishes_t *fishes, time_handler_t *clock, cartesian_point_t aquarium_dimensions);

fishes_t * new_fishes_from_config(config_parser_t *config_parser);


#endif //AQUARIUM_FISHES_H
