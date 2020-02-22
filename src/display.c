#include <SDL2/SDL_image.h>
#include "display.h"
#include "object.h"
#include "fish.h"
#include "memory_handling.h"

static struct SDL_Window *create_window(display_initial_data_t *display_initial_data);

static struct SDL_Renderer *create_renderer(struct SDL_Window *window, display_initial_data_t *
display_initial_data);

static inline bool are_textures_loaded(bool condition);

static inline bool are_textures_not_loaded(bool condition);

static bool load_textures(display_t *new_display, display_initial_data_t *display_initial_data);

static inline bool is_image_loaded(void *image);

static inline bool is_image_not_loaded(void *image);

static display_t * delete_failed_display(display_t * this, display_error_code_t error_code);

display_t *new_display(display_initial_data_t *display_initial_data) {
    display_t *this = new_object(sizeof(display_t));
    if (is_not_created(this)) {
        return delete_failed_display(this, DISPLAY_FAIL);
    }
    if (SDL_Init(display_initial_data->sdl_initialization_flags) != 0) {
        return delete_failed_display(this, SDL_INIT_FAIL);
    }
    this->window = create_window(display_initial_data);
    if (is_not_created(this->window)) {
        return delete_failed_display(this, WINDOW_FAIL);
    }
    this->renderer = create_renderer(this->window, display_initial_data);
    if (is_not_created(this->renderer)) {
        return delete_failed_display(this, RENDERER_FAIL);
    }
    if (are_textures_not_loaded(load_textures(this, display_initial_data))) {
        return delete_failed_display(this, IMAGE_FAIL);
    }

    return this;
}

static display_t * delete_failed_display(display_t * this, display_error_code_t error_code) {
    switch (error_code) {
        case DISPLAY_FAIL:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case SDL_INIT_FAIL:
            IMPLICIT_ERROR_MESSAGE("SDL error_code due to: %s", SDL_GetError());
            break;
        case WINDOW_FAIL:
            NEW_OBJECT_FAILURE("SDL_Window");
            break;
        case RENDERER_FAIL:
            NEW_OBJECT_FAILURE("struct SDL_Renderer");
            break;
        case IMAGE_FAIL:
            NEW_OBJECT_FAILURE("SDL_Texture");
            break;
        default:
            break;
    }
    return delete_display(this);
}


static struct SDL_Window *create_window(display_initial_data_t *display_initial_data) {
    return SDL_CreateWindow(
            display_initial_data->window_name,
            display_initial_data->window_dimensions.x,
            display_initial_data->window_dimensions.y,
            display_initial_data->window_dimensions.w,
            display_initial_data->window_dimensions.h,
            display_initial_data->window_flags);
}

static struct SDL_Renderer *create_renderer(struct SDL_Window *window, display_initial_data_t *
display_initial_data) {
    const int default_index_of_first_available_driver = -1;
    return SDL_CreateRenderer(window, default_index_of_first_available_driver, display_initial_data->renderer_flags);
}

static inline bool are_textures_loaded(bool condition) {
    return condition == true;
}

static inline bool are_textures_not_loaded(bool condition) {
    return !are_textures_loaded(condition);
}

static bool load_textures(display_t *new_display, display_initial_data_t *display_initial_data) {
    new_display->background_image = IMG_LoadTexture(new_display->renderer,
                                                    display_initial_data->background_image_filename);
    if (is_image_not_loaded(new_display->background_image)) {
        EXPLICIT_ERROR_MESSAGE("Couldn't load image: %s\n", display_initial_data->background_image_filename);
        return false;
    }

    new_display->fish_image = IMG_LoadTexture(new_display->renderer, display_initial_data->fish_image_filename);
    if (is_image_not_loaded(new_display->fish_image)) {
        EXPLICIT_ERROR_MESSAGE("Couldn't load image: %s\n", display_initial_data->fish_image_filename);
        return false;
    }

    return true;
}

static inline bool is_image_loaded(void *image) {
    return image != NULL;
}

static inline bool is_image_not_loaded(void *image) {
    return !is_image_loaded(image);
}

display_t *delete_display(display_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    if (is_created(this->fish_image)) {
        SDL_DestroyTexture(this->fish_image);
        this->fish_image = NULL;
    }
    if (is_created(this->background_image)) {
        SDL_DestroyTexture(this->background_image);
        this->background_image = NULL;
    }
    if (is_created(this->renderer)) {
        SDL_DestroyRenderer(this->renderer);
        this->renderer = NULL;
    }
    if (is_created(this->window)) {
        SDL_DestroyWindow(this->window);
        this->window = NULL;
    }
    if (IS_ALL_DELETED(this->fish_image, this->background_image, this->renderer, this->window)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE(DISPLAY_T_NAME);
        exit(2);
    }
}

void show_aquarium_contents(display_t *this, fishes_t *fishes, time_handler_t *clock) {
    int current_width, current_height;
    SDL_GetWindowSize(this->window, &current_width, &current_height);
    SDL_RenderSetLogicalSize(this->renderer, current_width, current_height);

    SDL_RenderCopy(this->renderer, this->background_image, NULL, NULL);

    fish_t *fish = NULL;
    dl_list_foreach(fishes, fish) {
        update_fish_dimensions(fish);
        show_fish(this->renderer, fish, this->fish_image, &fish->dimensions, clock);
    }
    SDL_RenderPresent(this->renderer);
}


