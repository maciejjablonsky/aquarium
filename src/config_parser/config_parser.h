//
// Created by foreverhungry on 18.02.2020.
//

#ifndef AQUARIUM_CONFIG_PARSER_H
#define AQUARIUM_CONFIG_PARSER_H
#include <stdio.h>
#include <stdbool.h>
#include <linux/limits.h>
#include "xml_tree.h"

#define WINDOW_NAME_MAX 128

typedef struct {
    size_t number_of_fishes;
    long double translational_velocity;
    long double harmonic_motion_amplitude;
    long double harmonic_motion_period;
    long double texture_display_scale;
    size_t window_width;
    size_t window_height;
    size_t window_x;
    size_t window_y;
    char window_name[WINDOW_NAME_MAX];
    bool is_window_fullscreen;
    bool is_window_resizable;
    char ordinary_fish_image_path[PATH_MAX];
    char background_image_path[PATH_MAX];
} config_data_t;

typedef struct {
    config_data_t * data;
    xml_tree_t * tree;
} config_t;

#endif //AQUARIUM_CONFIG_PARSER_H
