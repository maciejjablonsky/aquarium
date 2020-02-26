#ifndef AQUARIUM_DISPLAY_H
#define AQUARIUM_DISPLAY_H

#include <stdbool.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "config_parser/config_parser.h"
#include "fishes/fishes.h"

#define DISPLAY_XML_NODE    TO_XML_STRING("display")
#define WINDOW_XML_NODE     TO_XML_STRING("window")
#define GRAPHICS_XML_NODE   TO_XML_STRING("graphics")
#define FULLSCREEN_XML_NODE TO_XML_STRING("fullscreen")
#define RESIZABLE_XML_NODE  TO_XML_STRING("resizable")

typedef struct {
    SDL_Rect window_dimensions;
    bool is_fullscreen;
    Uint32 window_flags;
    Uint32 renderer_flags;
    Uint32 sdl_initialization_flags;
    char background_image_filename[FILENAME_MAX];
    char fish_image_filename[FILENAME_MAX];
    char window_name[FILENAME_MAX];
} display_initial_data_t;

typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * background_image;
    SDL_Texture * fish_image;
} display_t;
#define DISPLAY_T_NAME "display_t"



display_t * new_display(display_initial_data_t * display_initial_data);

void show_aquarium_contents(display_t *this, fishes_t *fishes, time_handler_t *clock);

display_t * delete_display(display_t * this);

display_t *new_display_from_config(config_parser_t *config_parser);

#endif //AQUARIUM_DISPLAY_H
