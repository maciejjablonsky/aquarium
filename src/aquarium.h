#ifndef AQUARIUM_AQUARIUM_H
#define AQUARIUM_AQUARIUM_H
#include "display/display.h"
#include "fishes/fishes.h"
#include "time_handler.h"
#include "config_parser/config_parser.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_events.h>
#include <libxml/tree.h>

#define AQUARIUM_XML_NODE TO_XML_STRING("aquarium")


typedef struct  {
    display_t * display;
    fishes_t * fishes;
    time_handler_t * clock;
    bool action;
} aquarium_t;
#define AQUARIUM_T_NAME "aquarium_t"

aquarium_t *new_aquarium(display_initial_data_t *display_initial_data, fishes_initial_data_t *fish_initial_data,
                         size_t amount_of_fishes);
aquarium_t *new_aquarium_from_config(config_parser_t *config_parser);
aquarium_t * delete_aquarium(aquarium_t * this);

void run_aquarium(aquarium_t * aquarium);
bool start_action(aquarium_t * aquarium);
bool stop_action(aquarium_t * aquarium);
bool is_aquarium_running(aquarium_t * aquarium);
void handle_events(aquarium_t * aquarium);
void dispatch_event(aquarium_t * aquarium, SDL_Event * sdl_event);
void dispatch_pressed_key(aquarium_t *aquarium, SDL_Event *sdl_event);
cartesian_point_t get_aquarium_dimensions(aquarium_t * aquarium);
#endif //AQUARIUM_AQUARIUM_H
