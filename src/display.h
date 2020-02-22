#ifndef AQUARIUM_DISPLAY_H
#define AQUARIUM_DISPLAY_H

#include <stdbool.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "fishes.h"


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

typedef enum {
   DISPLAY_FAIL, SDL_INIT_FAIL, WINDOW_FAIL, RENDERER_FAIL, IMAGE_FAIL
} display_error_code_t;

display_t * new_display(display_initial_data_t * display_initial_data);

void show_aquarium_contents(display_t *this, fishes_t *fishes, time_handler_t *clock);

display_t * delete_display(display_t * this);

#endif //AQUARIUM_DISPLAY_H
