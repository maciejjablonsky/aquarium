//
// Created by foreverhungry on 13.02.2020.
//

#define _USE_MATH_DEFINES
#include <time.h>
#include <SDL2/SDL.h>
#include "aquarium.h"
#include "graphics_paths.h"

void set_display_initial_data(display_initial_data_t *data);

void set_fishes_initial_data(fishes_initial_data_t *data);

#define AQUARIUM_WIDTH 1080
#define AQUARIUM_HEIGHT 720


int main() {
    srand(time(NULL));
    display_initial_data_t display_initial_data;
    set_display_initial_data(&display_initial_data);
    fishes_initial_data_t fishes_initial_data;
    set_fishes_initial_data(&fishes_initial_data);
    size_t amount_of_fishes =300;

    aquarium_t *aquarium = create_aquarium(&display_initial_data, &fishes_initial_data, amount_of_fishes);
    run_aquarium(aquarium);
    destroy_aquarium(aquarium);
}


void set_display_initial_data(display_initial_data_t *data) {
    SDL_Rect window_dimensions = {50, 50, AQUARIUM_WIDTH, AQUARIUM_HEIGHT};
    data->window_dimensions = window_dimensions;
    data->sdl_initialization_flags = SDL_INIT_VIDEO;
    strcpy(data->window_name, "Aquarium");
    data->window_flags = SDL_WINDOW_RESIZABLE;
    data->renderer_flags = SDL_RENDERER_ACCELERATED;
    strcpy(data->background_image_filename, BACKGROUND_IMAGE_PATH);
    strcpy(data->fish_image_filename, FISH_IMAGE_PATH);
}

void set_fishes_initial_data(fishes_initial_data_t *data) {
    data->fish_scale = 30;
    SDL_Rect dimensions = {0, 0, data->fish_scale * 1.641, data->fish_scale};
    data->fish_dimensions = dimensions;
    data->initial_translational_velocity = 200;
    data->amplitude = 5;
    data->wave_movement_period = 1;
    data->max_x = AQUARIUM_WIDTH;
    data->max_y = AQUARIUM_HEIGHT;
}
