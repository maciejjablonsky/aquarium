//
// Created by foreverhungry on 13.02.2020.
//

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include "time_handler.h"
#include "object.h"
#include "memory_handling.h"

time_handler_t *new_time_handler(void) {
    time_handler_t * this = new_object(sizeof(time_handler_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }
    reset_time_handler(this);
    return this;
}

time_handler_t *delete_time_handler(time_handler_t *handler) {
    return delete_object(handler);
}

void reset_time_handler(time_handler_t *handler) {
    handler->dtime = 0;
    handler->global = 0;
}

void update_time_handler(time_handler_t *handler) {
    static Uint32 prev_ticks = 0;
    Uint32 next_ticks = SDL_GetTicks();
    handler->dtime = (next_ticks - prev_ticks) * 0.001;
    prev_ticks = next_ticks;
    handler->global += handler->dtime;
}

long double get_dtime(time_handler_t *handler) {
    return handler->dtime;
}

long double get_global_time(time_handler_t *handler) {
    return handler->global;
}

