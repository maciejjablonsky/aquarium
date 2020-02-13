//
// Created by foreverhungry on 13.02.2020.
//

#include "../include/fishes.h"
#include "../include/fish.h"
#include "../include/object.h"
#include "../include/time_handler.h"
#include "../include/wall.h"



fishes_t *create_fishes(fishes_initial_data_t *fishes_initial_data, size_t amount_of_fishes) {
    fishes_t *this = DL_LIST_create(sizeof(fish_t), DL_COPY_POINTER, (void *(*)(void *))destroy_fish);
    if (is_not_created(this)) {
        fprintf(stderr, "Failed to create fishes.\n");
        exit(1);
    }

    for (size_t i = 0; i < amount_of_fishes; ++i) {
        fish_t *new_fish = create_fish(fishes_initial_data->max_x, fishes_initial_data->max_y,
                                       fishes_initial_data->fish_dimensions,
                                       fishes_initial_data->initial_translational_velocity,
                                       fishes_initial_data->amplitude, fishes_initial_data->wave_movement_period);
        if (is_not_created(new_fish)) {
            fprintf(stderr, "Failed to create %lu fish.\n", i + 1);
            exit(1);
        }
        DL_LIST_add_item(this, new_fish);
    }
    return this;
}

void move_each_fish_in_aquarium(fishes_t *fishes, time_handler_t *clock, cartesian_point_t aquarium_dimensions) {
    fish_t * fish = NULL;
    dl_list_foreach(fishes, fish) {
        update_velocity_of(fish, clock);
        wall_t wall_to_hit = which_wall_is_going_to_hit(fish, &aquarium_dimensions);
        if (is_going_to_hit_wall(wall_to_hit)) {
            dodge_wall(fish, wall_to_hit);
        }
        update_fish_coordinates(fish);
    }
}

fishes_t *destroy_fishes(fishes_t *this) {
    return DL_LIST_delete(this);
}

void multiply_fishes_velocity(fishes_t *this, long double multiplier) {
    for (size_t i = 0; i < DL_LIST_size(this); ++i)
    {
        fish_t *fish = DL_LIST_item_at(this, i);
        multiply_fish_velocity(fish, multiplier);
    }
}
