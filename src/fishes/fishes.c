#include "fishes.h"
#include "fish.h"
#include "object.h"
#include "time_handler.h"
#include "wall.h"
#include "memory_handling.h"
#include "exit_codes.h"

typedef enum {
    FISHES_NO_MEMORY, FISHES_FISH_FAIL
} fishes_error_code_t;

static fishes_t *delete_error_fishes(fishes_t *this, fishes_error_code_t error_code);

fishes_t *initialize_fishes_from_config(fishes_t *this, config_parser_t *config_parser);

fishes_t *new_fishes(fishes_initial_data_t *fish_initial_data, size_t amount_of_fishes) {
    fishes_t *this = new_DL_LIST(sizeof(fish_t), DL_COPY_POINTER, (void *(*)(void *)) delete_fish);
    if (is_not_created(this)) {
        return delete_error_fishes(this, FISHES_NO_MEMORY);
    }

    for (size_t i = 0; i < amount_of_fishes; ++i) {
        fish_t *fish = new_fish(fish_initial_data->max_x, fish_initial_data->max_y,
                                fish_initial_data->fish_dimensions,
                                fish_initial_data->initial_translational_velocity,
                                fish_initial_data->amplitude, fish_initial_data->wave_movement_period);
        if (is_not_created(fish)) {
            return delete_error_fishes(this, FISHES_FISH_FAIL);
        }
        DL_LIST_add_item(this, fish);
    }
    return this;
}

fishes_t *new_fishes_from_config(config_parser_t *config_parser) {
    fishes_t *this = new_DL_LIST(sizeof(fish_t), DL_COPY_POINTER, (void *(*)(void *)) delete_fish);
    if (is_not_created(this)) {
        return delete_error_fishes(this, FISHES_NO_MEMORY);
    }
    initialize_fishes_from_config(this, config_parser);
    return this;
}

fishes_t *initialize_fishes_from_config(fishes_t *this, config_parser_t *config_parser) {
    char *xpath = "fishes";
    config_xml_eval_xpath(config_parser, xpath);
    xpath = "number_of_fishes";
    xmlChar *value = config_xml_get_value_with_xpath(config_parser, xpath);
    size_t number_of_fishes = strtoul(value, NULL, 10);
    for (size_t i = 0; i < number_of_fishes; ++i) {
        fish_t *fish = new_fish_from_config(config_parser);
        DL_LIST_add_item(this, fish);
    }
    config_xml_eval_xpath(config_parser, "..");
    return this;
}

static fishes_t *delete_error_fishes(fishes_t *this, fishes_error_code_t error_code) {
    switch (error_code) {
        case FISHES_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case FISHES_FISH_FAIL:
            NEW_OBJECT_FAILURE(FISH_T_NAME);
            break;
        default:
            break;
    }
    return delete_fishes(this);
}

void move_each_fish_in_aquarium(fishes_t *fishes, time_handler_t *clock, cartesian_point_t aquarium_dimensions) {
    fish_t *fish = NULL;
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
    void *ret = delete_DL_LIST(this);
    if (is_deleted(ret)) {
        return NULL;
    } else {
        DELETE_OBJECT_FAILURE(FISHES_T_NAME);
        exit(EXIT_FREE_MEMORY_ERROR);
    }
}

void multiply_fishes_velocity(fishes_t *this, long double multiplier) {
    for (size_t i = 0; i < DL_LIST_size(this); ++i) {
        fish_t *fish = DL_LIST_item_at(this, i);
        multiply_fish_translational_velocity(fish, multiplier);
    }
}


