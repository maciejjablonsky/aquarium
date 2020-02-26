#include <SDL2/SDL_image.h>
#include "display.h"
#include "object.h"
#include "fishes/fish.h"
#include "memory_handling.h"
#include "exit_codes.h"

typedef enum {
    DISPLAY_FAIL, DISPLAY_SDL_INIT_FAIL, DISPLAY_WINDOW_FAIL, DISPLAY_RENDERER_FAIL, DISPLAY_IMAGE_FAIL
} display_error_code_t;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char *name;
    Uint32 flags;
} window_config_data_t;

static struct SDL_Window *create_window(display_initial_data_t *display_initial_data);

static struct SDL_Renderer *create_renderer(struct SDL_Window *window, display_initial_data_t *
display_initial_data);

static inline bool are_textures_loaded(bool condition);

static inline bool are_textures_not_loaded(bool condition);

static bool load_textures(display_t *new_display, display_initial_data_t *display_initial_data);

static inline bool is_image_loaded(void *image);

static inline bool is_image_not_loaded(void *image);

static display_t *delete_failed_display(display_t *this, display_error_code_t error_code);

static display_t *initialize_display_from_config(display_t *this, config_parser_t *config_parser);

static SDL_Renderer *new_sdl_renderer_from_config(config_parser_t *config_parser, SDL_Window *window);

static SDL_Window *new_sdl_window_from_config(config_parser_t *config_parser);

static window_config_data_t *load_window_config_data(config_parser_t *config_parser);

static window_config_data_t *delete_window_config_data(window_config_data_t *data);

static bool load_textures_from_config(display_t *this, config_parser_t *config_parser);

static SDL_Texture *load_texture_from_config_with_xpath(config_parser_t *config_parser, const char *xpath,
                                                        SDL_Renderer *
                                                        renderer);

display_t *new_display(display_initial_data_t *display_initial_data) {
    display_t *this = new_object(sizeof(display_t));
    if (is_not_created(this)) {
        return delete_failed_display(this, DISPLAY_FAIL);
    }
    if (SDL_Init(display_initial_data->sdl_initialization_flags) != 0) {
        return delete_failed_display(this, DISPLAY_SDL_INIT_FAIL);
    }
    this->window = create_window(display_initial_data);
    if (is_not_created(this->window)) {
        return delete_failed_display(this, DISPLAY_WINDOW_FAIL);
    }
    this->renderer = create_renderer(this->window, display_initial_data);
    if (is_not_created(this->renderer)) {
        return delete_failed_display(this, DISPLAY_RENDERER_FAIL);
    }
    if (are_textures_not_loaded(load_textures(this, display_initial_data))) {
        return delete_failed_display(this, DISPLAY_IMAGE_FAIL);
    }

    return this;
}

display_t *new_display_from_config(config_parser_t *config_parser) {
    display_t *this = new_object(sizeof(display_t));
    if (is_not_created(this)) {
        return delete_failed_display(this, DISPLAY_FAIL);
    }
    initialize_display_from_config(this, config_parser);
    return this;
}

display_t *initialize_display_from_config(display_t *this, config_parser_t *config_parser) {
    config_xml_eval_xpath(config_parser, DISPLAY_XML_NODE);
    this->window = new_sdl_window_from_config(config_parser);
    if (is_not_created(this->window)) {
        return delete_failed_display(this, DISPLAY_WINDOW_FAIL);
    }
    this->renderer = new_sdl_renderer_from_config(config_parser, this->window);
    if (is_not_created(this->renderer)) {
        return delete_failed_display(this, DISPLAY_RENDERER_FAIL);
    }
    if (are_textures_not_loaded(load_textures_from_config(this, config_parser))) {
        return delete_failed_display(this, DISPLAY_IMAGE_FAIL);
    }
    config_xml_eval_xpath(config_parser, "..");
    return this;
}

SDL_Window *new_sdl_window_from_config(config_parser_t *config_parser) {
    config_xml_eval_xpath(config_parser, WINDOW_XML_NODE);
    window_config_data_t *data = load_window_config_data(config_parser);
    config_xml_eval_xpath(config_parser, "..");
    struct SDL_Window *window = SDL_CreateWindow(
            data->name, data->x, data->y, data->width, data->height, data->flags);
    delete_window_config_data(data);
    return window;
}

window_config_data_t *load_window_config_data(config_parser_t *config_parser) {
    window_config_data_t *data = new_object(sizeof(window_config_data_t));
    if (is_not_created(data)) {
        NEW_OBJECT_FAILURE("window_config_data_t");
        exit(EXIT_NO_MEMORY);
    }

    char *xpath = "dimensions/width";
    xmlChar *value = config_xml_get_value_with_xpath(config_parser, xpath);
    data->width = strtoimax(value, NULL, 10);
    xmlFree(value);

    xpath = "dimensions/height";
    value = config_xml_get_value_with_xpath(config_parser, xpath);
    data->height = strtoimax(value, NULL, 10);
    xmlFree(value);

    xpath = "position/x";
    value = config_xml_get_value_with_xpath(config_parser, xpath);
    data->x = strtoimax(value, NULL, 10);
    xmlFree(value);

    xpath = "position/y";
    value = config_xml_get_value_with_xpath(config_parser, xpath);
    data->y = strtoimax(value, NULL, 10);
    xmlFree(value);

    xpath = "name";
    value = config_xml_get_value_with_xpath(config_parser, xpath);
    data->name = value;

    xpath = "fullscreen";
    data->flags = 0;
    value = config_xml_get_value_with_xpath(config_parser, xpath);
    if (xmlStrcmp(value, "yes") == 0) {
        data->flags |= SDL_WINDOW_FULLSCREEN;
    }
    xmlFree(value);

    xpath = "resizable";
    value = config_xml_get_value_with_xpath(config_parser, xpath);
    if (xmlStrcmp(value, "yes") == 0) {
        data->flags |= SDL_WINDOW_RESIZABLE;
    }
    xmlFree(value);

    return data;
}

window_config_data_t *delete_window_config_data(window_config_data_t *data) {
    if (is_created(data->name)) {
        delete_object(data->name);
    }
    return delete_object(data);
}

SDL_Renderer *new_sdl_renderer_from_config(config_parser_t *config_parser, SDL_Window *window) {
    Uint32 flags = 0;
    char *xpath = "renderer/accelerated";
    xmlChar *value = config_xml_get_value_with_xpath(config_parser, xpath);
    if (xmlStrcmp(value, "yes") == 0) {
        flags |= SDL_RENDERER_ACCELERATED;
    }
    xmlFree(value);
    const int default_index_of_first_available_driver = -1;
    return SDL_CreateRenderer(window, default_index_of_first_available_driver, flags);
}

bool load_textures_from_config(display_t *this, config_parser_t *config_parser) {
    char *xpath = "graphics/paths";
    config_xml_eval_xpath(config_parser, xpath);
    this->background_image = load_texture_from_config_with_xpath(config_parser, "background", this->renderer);
    this->fish_image = load_texture_from_config_with_xpath(config_parser, "ordinary_fish", this->renderer);
    config_xml_eval_xpath(config_parser, "../..");
    return true;
}

SDL_Texture *load_texture_from_config_with_xpath(config_parser_t *config_parser, const char *xpath, SDL_Renderer *
renderer) {
    char *value = config_xml_get_value_with_xpath(config_parser, xpath);
    SDL_Texture *texture = IMG_LoadTexture(renderer, value);
    if (is_not_created(texture)) {
        EXPLICIT_ERROR_MESSAGE("Cannot load %s texture.", value);
        exit(EXIT_XML_PARSING_ERROR);
    }
    return texture;

}

static display_t *delete_failed_display(display_t *this, display_error_code_t error_code) {
    switch (error_code) {
        case DISPLAY_FAIL:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case DISPLAY_SDL_INIT_FAIL:
            IMPLICIT_ERROR_MESSAGE("SDL error_code due to: %s", SDL_GetError());
            break;
        case DISPLAY_WINDOW_FAIL:
            NEW_OBJECT_FAILURE("SDL_Window");
            break;
        case DISPLAY_RENDERER_FAIL:
            NEW_OBJECT_FAILURE("struct SDL_Renderer");
            break;
        case DISPLAY_IMAGE_FAIL:
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
        EXPLICIT_ERROR_MESSAGE("Couldn't load image: %s", display_initial_data->background_image_filename);
        exit(EXIT_IMAGE_NOT_FOUND);
    }

    new_display->fish_image = IMG_LoadTexture(new_display->renderer, display_initial_data->fish_image_filename);
    if (is_image_not_loaded(new_display->fish_image)) {
        EXPLICIT_ERROR_MESSAGE("Couldn't load image: %s", display_initial_data->fish_image_filename);
        exit(EXIT_IMAGE_NOT_FOUND);
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
        exit(EXIT_FREE_MEMORY_ERROR);
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


