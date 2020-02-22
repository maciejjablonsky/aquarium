#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include "time_handler.h"
#include "object.h"
#include "memory_handling.h"

typedef enum {
    TRANSLATIONAL_MOTION_NO_MEMORY
} time_handler_error_code_t;

static time_handler_t * delete_error_time_handler(time_handler_t * this, time_handler_error_code_t error_code);

time_handler_t *new_time_handler(void) {
    time_handler_t * this = new_object(sizeof(time_handler_t));
    if (is_not_created(this)) {
        return delete_error_time_handler(this, TRANSLATIONAL_MOTION_NO_MEMORY);
    }
    reset_time_handler(this);
    return this;
}

time_handler_t *delete_time_handler(time_handler_t *handler) {
    return delete_object(handler);
}

static time_handler_t * delete_error_time_handler(time_handler_t * this, time_handler_error_code_t error_code) {
    switch(error_code) {
        case TRANSLATIONAL_MOTION_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        default:
            break;
    }
    return delete_time_handler(this);
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

