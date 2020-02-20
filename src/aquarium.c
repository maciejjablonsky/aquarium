//
// Created by foreverhungry on 12.02.2020.
//

#include <SDL2/SDL_events.h>
#include "aquarium.h"
#include "object.h"
#include "cartesian_point.h"
#include "memory_handling.h"

static bool is_aquarium_created_properly(aquarium_t * this) {
    if (is_not_created(this->fishes)) {
        NEW_OBJECT_FAILURE("fishes_t");
    } else if (is_not_created(this->display)) {
        NEW_OBJECT_FAILURE("display_t");
    } else if (is_not_created(this->clock)) {
        NEW_OBJECT_FAILURE("time_handler_t");
    }
    return this->fishes && this->display && this->clock;
}

aquarium_t *new_aquarium(display_initial_data_t *display_initial_data, fishes_initial_data_t *fish_initial_data,
                         size_t amount_of_fishes) {
    aquarium_t *this = new_object(sizeof(aquarium_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }

    this->display = new_display(display_initial_data);
    this->fishes = new_fishes(fish_initial_data, amount_of_fishes);
    this->clock = new_time_handler();
    this->action = false;
    if (is_aquarium_created_properly(this)) {
        return this;
    } else{
        return delete_aquarium(this);
    }
}

aquarium_t *delete_aquarium(aquarium_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    this->fishes = delete_fishes(this->fishes);
    this->display = delete_display(this->display);
    this->clock = delete_time_handler(this->clock);
    if (is_all_deleted(3, this->fishes, this->display, this->clock)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE("* aquarium_t");
        return this;
    }
}

void run_aquarium(aquarium_t *aquarium) {
    reset_time_handler(aquarium->clock);
    start_action(aquarium);
    update_time_handler(aquarium->clock);
    while (is_aquarium_running(aquarium)) {
        handle_events(aquarium);
        update_time_handler(aquarium->clock);
        move_each_fish_in_aquarium(aquarium->fishes, aquarium->clock, get_aquarium_dimensions(aquarium));
        show_aquarium_contents(aquarium->display, aquarium->fishes, aquarium->clock);
    }
}

bool start_action(aquarium_t *aquarium) {
    return aquarium->action = true;
}

bool stop_action(aquarium_t *aquarium) {
    return aquarium->action = false;
}

bool is_aquarium_running(aquarium_t *aquarium) {
    return aquarium->action;
}

void handle_events(aquarium_t *aquarium) {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event) > 0)
    {
        dispatch_event(aquarium, &sdl_event);
    }
}

void dispatch_event(aquarium_t *aquarium, SDL_Event *sdl_event) {
    switch (sdl_event->type)
    {
        case SDL_QUIT:
            stop_action(aquarium);
            break;
        case SDL_KEYDOWN:
            dispatch_pressed_key(aquarium, sdl_event);
            break;
    }
}

void dispatch_pressed_key(aquarium_t *aquarium, SDL_Event *sdl_event) {
    switch (sdl_event->key.keysym.sym)
    {
        case SDLK_ESCAPE:
            stop_action(aquarium);
            break;
        case SDLK_UP:
            multiply_fishes_velocity(aquarium->fishes, 1.2);
            break;
        case SDLK_DOWN:
            multiply_fishes_velocity(aquarium->fishes, 0.8);
            break;
        default:
            break;
    }
}

cartesian_point_t get_aquarium_dimensions(aquarium_t *aquarium) {
    cartesian_point_t dimensions;
    int x, y;
    SDL_GetWindowSize(aquarium->display->window, &x, &y);
    dimensions.x = x;
    dimensions.y = y;
    return dimensions;
}
