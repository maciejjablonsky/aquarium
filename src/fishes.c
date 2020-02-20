//
// Created by foreverhungry on 13.02.2020.
//

#include "fishes.h"
#include "fish.h"
#include "object.h"
#include "time_handler.h"
#include "wall.h"



fishes_t *new_fishes(fishes_initial_data_t *fish_initial_data, size_t amount_of_fishes) {
    fishes_t *this = new_DL_LIST(sizeof(fish_t), DL_COPY_POINTER, (void *(*)(void *)) delete_fish);
    if (is_not_created(this)) {
        NEW_OBJECT_FAILURE(*fishes_t);
        return NULL;
    }

    for (size_t i = 0; i < amount_of_fishes; ++i) {
        fish_t *fish = new_fish(fish_initial_data->max_x, fish_initial_data->max_y,
                                fish_initial_data->fish_dimensions,
                                fish_initial_data->initial_translational_velocity,
                                fish_initial_data->amplitude, fish_initial_data->wave_movement_period);
        if (is_not_created(fish)) {
            NEW_OBJECT_FAILURE(*fish_t);
            return delete_fishes(this);
        }
        DL_LIST_add_item(this, fish);
    }
    return this;
}

void move_each_fish_in_aquarium(fishes_t *fishes, time_handler_t *clock, cartesian_point_t aquarium_dimensions) {
    fish_t * fish = NULL;
    dl_list_foreach(fishes, fish) {
        update_delta_general_velocity(fish, clock);
        wall_t wall_to_hit = which_wall_is_going_to_hit(fish, &aquarium_dimensions);
        if (is_going_to_hit_wall(wall_to_hit)) {
            dodge_wall(fish, wall_to_hit, clock);
        }
        update_fish_coordinates(fish);
    }
}

fishes_t *delete_fishes(fishes_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    void * ret = delete_DL_LIST(this);
    if (is_deleted(ret)) {
        return NULL;
    } else {
        DELETE_OBJECT_FAILURE("* fishes_t");
        return this;
    }
}

void multiply_fishes_velocity(fishes_t *this, long double multiplier) {
    for (size_t i = 0; i < DL_LIST_size(this); ++i)
    {
        fish_t *fish = DL_LIST_item_at(this, i);
        multiply_fish_translational_velocity(fish, multiplier);
    }
}
