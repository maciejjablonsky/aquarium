#include "fish.h"
#include "object.h"
#include "wall.h"
#include "time_handler.h"
#include "degrees.h"
#include "random.h"
#include "memory_handling.h"
#include <math.h>
#include "exit_codes.h"

typedef enum {
    FISH_NO_MEMORY, FISH_COORDS_FAIL, FISH_TRANSLATIONAL_MOTION_FAIL, FISH_HARMONIC_MOTION_FAIL,
    FISH_GENERAL_VELOCITY_FAIL
} fish_error_code_t;

static fish_t *delete_failed_fish(fish_t *this, fish_error_code_t error_code);

static fish_error_code_t get_fish_error_code(fish_t *this);

static fish_t *initialize_fish_from_config(fish_t *this, config_parser_t *config_parser);

fish_t *new_fish(size_t max_x, size_t max_y, SDL_Rect
dimensions, long double initial_translational_velocity, long double wave_amplitude, long double wave_period) {
    fish_t *this = new_object(sizeof(fish_t));
    if (is_not_created(this)) {
        return delete_failed_fish(this, FISH_NO_MEMORY);
    }

    this->z_motion_phase = ldto_radians(ldrandom_range(0, 360));
    long double x_0 = dimensions.x;//ldrandom_range(0, max_x);
    long double y_0 = dimensions.y;// ldrandom_range(0, max_y);
    this->coords = new_cartesian_point(x_0, y_0);


    long double translational_angle = ldto_radians(ldrandom_range(0, 360));
    long double v_x = initial_translational_velocity * cosl(translational_angle);
    long double v_y = initial_translational_velocity * sinl(translational_angle);

    this->translational_motion = new_translational_motion(x_0, y_0,
                                                          initial_translational_velocity, translational_angle,
                                                          translational_angle);
    long double harmonic_angle = translational_angle + M_PI / 2;
    this->harmonic_motion = new_harmonic_motion(wave_amplitude, wave_period,
                                                ldto_radians(ldrandom_range(0, 360)),
                                                harmonic_angle);

    this->general_velocity = new_cartesian_velocity(0, 0);
    this->dimensions = dimensions;

    if (IS_ALL_CREATED(this->coords, this->translational_motion, this->harmonic_motion, this->general_velocity)) {
        return this;
    } else {
        return delete_failed_fish(this, get_fish_error_code(this));
    }
}

fish_t *new_fish_from_config(config_parser_t *config_parser) {
    fish_t *this = new_object(sizeof(fish_t));
    // TODO ended here
    initialize_fish_from_config(this, config_parser);
    return this;

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-sign"
fish_t *initialize_fish_from_config(fish_t *this, config_parser_t *config_parser) {
    char *xpath = "fish";
    config_xml_eval_xpath(config_parser, "fish");
    this->z_motion_phase = ldto_radians(ldrandom_range(0, 360));
    int max_x = strtoimax(config_xml_get_value_with_xpath(config_parser, "../../display//dimensions/width"),
            NULL, 10);
    int max_y = strtoimax(config_xml_get_value_with_xpath(config_parser, "../../display//dimensions/height"),
            NULL, 10);
    long double x_0 = ldrandom_range(0, max_x);
    long double y_0 = ldrandom_range(0, max_y);
    this->coords = new_cartesian_point(x_0, y_0);

    long double itranslational_v = strtold(config_xml_get_value_with_xpath(config_parser, "translational_motion/initial_translational_velocity"),
            NULL);
    long double translational_angle = ldto_radians(ldrandom_range(0, 360));
    long double v_x = itranslational_v * cosl(translational_angle);
    long double v_y = itranslational_v * sinl(translational_angle);
    this->translational_motion = new_translational_motion(x_0, y_0,
                                                          itranslational_v, translational_angle,
                                                          translational_angle);

    long double harmonic_angle = translational_angle + M_PI / 2;
    long double harmonic_amplitude = strtold(config_xml_get_value_with_xpath(config_parser, "harmonic_motion/amplitude"), NULL);
    long double harmonic_period = strtold(config_xml_get_value_with_xpath(config_parser, "harmonic_motion/period"), NULL);
    this->harmonic_motion = new_harmonic_motion(harmonic_amplitude, harmonic_period,
                                                ldto_radians(ldrandom_range(0, 360)),
                                                harmonic_angle);
    this->general_velocity = new_cartesian_velocity(0, 0);

    long double ratio = strtold(config_xml_get_value_with_xpath(config_parser, "texture/width_to_height_ratio"), NULL);
    long double width = strtold(config_xml_get_value_with_xpath(config_parser, "texture/width"), NULL);
    this->dimensions.w = width;
    this->dimensions.h = width * ratio;
    config_xml_eval_xpath(config_parser, "..");
    return this;
}
#pragma clang diagnostic pop

static fish_t *delete_failed_fish(fish_t *this, fish_error_code_t error_code) {
    switch (error_code) {
        case FISH_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case FISH_COORDS_FAIL:
            NEW_OBJECT_FAILURE(CARTESIAN_POINT_T_NAME);
            break;
        case FISH_TRANSLATIONAL_MOTION_FAIL:
            NEW_OBJECT_FAILURE(TRANSLATIONAL_MOTION_T_NAME);
            break;
        case FISH_HARMONIC_MOTION_FAIL:
            NEW_OBJECT_FAILURE(HARMONIC_MOTION_T_NAME);
            break;
        case FISH_GENERAL_VELOCITY_FAIL:
            NEW_OBJECT_FAILURE(CARTESIAN_VELOCITY_T_NAME);
            break;
        default:
            break;
    }
    return delete_fish(this);
}

static fish_error_code_t get_fish_error_code(fish_t *this) {
    if (is_not_created(this)) return FISH_NO_MEMORY;
    else if (is_not_created(this->coords)) return FISH_COORDS_FAIL;
    else if (is_not_created(this->translational_motion)) return FISH_TRANSLATIONAL_MOTION_FAIL;
    else if (is_not_created(this->harmonic_motion)) return FISH_HARMONIC_MOTION_FAIL;
    else if (is_not_created(this->general_velocity)) return FISH_GENERAL_VELOCITY_FAIL;
    else return -1;
}


fish_t *delete_fish(fish_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    this->coords = delete_cartesian_point(this->coords);
    this->general_velocity = delete_cartesian_velocity(this->general_velocity);
    this->harmonic_motion = delete_harmonic_motion(this->harmonic_motion);
    this->translational_motion = delete_translational_motion((this->translational_motion));


    if (IS_ALL_DELETED(this->coords, this->general_velocity, this->translational_motion, this->harmonic_motion)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE(FISH_T_NAME);
        exit(EXIT_FREE_MEMORY_ERROR);
    }
}

void draw_directions(SDL_Renderer *renderer, fish_t *fish, SDL_Rect *fish_dimensions) {
    long double angle_to_x_axis = fish->translational_motion->polar_v->angle;
    long double length = 60;
    int x = length * cosl(angle_to_x_axis) + fish->coords->x + (long double) fish_dimensions->w / 2;
    int y = length * sinl(angle_to_x_axis) + fish->coords->y + (long double) fish_dimensions->h / 2;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderDrawLine(renderer, fish->coords->x + fish_dimensions->w / 2, fish->coords->y + fish_dimensions->h / 2, x,
                       y);
}

long double scale_factor_fish(fish_t *fish, time_handler_t *clock) {
    return sinl
                   (((2 * M_PI) / 1.25) *
                    clock->global + fish->z_motion_phase) * 0.02 + 1;
}

void show_fish(SDL_Renderer *renderer, fish_t *fish, SDL_Texture *fish_texture, SDL_Rect *fish_rectangle,
               time_handler_t *clock) {
    SDL_Point center = {fish_rectangle->w / 2, fish_rectangle->h / 2};
    long double angle = ldto_degrees(atan2l(fish->general_velocity->y, fish->general_velocity->x));

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    long double a_angle = fabsl(angle);

    if (90 <= a_angle && a_angle < 180) {
        flip = SDL_FLIP_VERTICAL;
    }
    SDL_Rect d_dimensions = {
            fish_rectangle->x,
            fish_rectangle->y,
            fish_rectangle->w * scale_factor_fish(fish, clock),
            fish_rectangle->h * scale_factor_fish(fish, clock),
    };
    //draw_directions(renderer, fish, fish_rectangle);
    SDL_RenderCopyEx(renderer, fish_texture, NULL, &d_dimensions, (double) angle, &center, flip);
}


void set_fish_velocity_value(fish_t *this, long double value) {
    set_translational_motion_velocity(this->translational_motion, value);
}

void multiply_fish_translational_velocity(fish_t *this, long double multiplier) {
    set_fish_velocity_value(this, get_translational_velocity_value(this->translational_motion) * multiplier);
}

wall_t which_wall_is_going_to_hit(fish_t *this, cartesian_point_t *aquarium_dimensions) {
    if (is_going_to_hit_top_wall(this)) {
        return TOP;
    } else if (is_going_to_hit_right_wall(this, aquarium_dimensions)) {
        return RIGHT;
    } else if (is_going_to_hit_bottom_wall(this, aquarium_dimensions)) {
        return BOTTOM;
    } else if (is_going_to_hit_left_wall(this)) {
        return LEFT;
    }
    return NONE;
}

bool is_going_to_hit_top_wall(fish_t *this) {
    return this->coords->y + get_y_general_velocity(this) <= 0;
}

bool is_going_to_hit_right_wall(fish_t *this, cartesian_point_t *aquarium_dimensions) {
    return this->coords->x + this->dimensions.w + get_x_general_velocity(this) >= aquarium_dimensions->x;
}

bool is_going_to_hit_bottom_wall(fish_t *this, cartesian_point_t *aquarium_dimensions) {
    return this->coords->y + this->dimensions.h + get_y_general_velocity(this) >= aquarium_dimensions->y;
}

bool is_going_to_hit_left_wall(fish_t *this) {
    return this->coords->x + get_x_general_velocity(this) <= 0;
}

void update_delta_general_velocity(fish_t *this, time_handler_t *clock) {
    update_translational_motion(this->translational_motion, clock->dtime);
    update_harmonic_motion(this->harmonic_motion, clock->dtime);
    update_x_delta_general_velocity(this);
    update_y_delta_general_velocity(this);
}

void update_x_delta_general_velocity(fish_t *this) {
    this->general_velocity->x =
            get_x_delta_translational_velocity(this->translational_motion) + get_x_delta_harmonic_velocity
                    (this->harmonic_motion);
}

void update_y_delta_general_velocity(fish_t *this) {
    this->general_velocity->y =
            get_y_delta_translational_velocity(this->translational_motion)
            + get_y_delta_harmonic_velocity(this->harmonic_motion);
}

long double get_general_velocity(fish_t *this) {
    return sqrtl(powl(this->general_velocity->x, 2) + powl(this->general_velocity->y, 2));
}

long double get_y_general_velocity(fish_t *this) {
    return this->general_velocity->y;
}

long double get_x_general_velocity(fish_t *this) {
    return this->general_velocity->x;
}


bool is_going_to_hit_wall(wall_t wall) {
    return wall != NONE;
}

void dodge_wall(fish_t *this, wall_t wall_to_hit, time_handler_t *clock) {
    if (wall_to_hit == TOP || wall_to_hit == BOTTOM) {
        dodge_horizontal_wall(this);
    } else {
        dodge_vertical_wall(this);
    }
    update_delta_general_velocity(this, clock);
}

void dodge_horizontal_wall(fish_t *this) {
    long double x = get_x_translational_velocity(this->translational_motion);
    long double y = -get_y_translational_velocity(this->translational_motion);
    long double angle = atan2l(y, x);
    set_translational_motion_angle(this->translational_motion, angle);
    set_harmonic_motion_angle(this->harmonic_motion, angle + M_PI / 2);
    suspend_harmonic_motion(this->harmonic_motion, count_suspend_time(this->harmonic_motion,
                                                                      get_x_translational_velocity(
                                                                              this->translational_motion)));
}

void dodge_vertical_wall(fish_t *this) {
    long double x = -get_x_translational_velocity(this->translational_motion);
    long double y = get_y_translational_velocity(this->translational_motion);
    long double angle = atan2l(y, x);

    set_translational_motion_angle(this->translational_motion, angle);
    set_harmonic_motion_angle(this->harmonic_motion, angle + M_PI / 2);
    suspend_harmonic_motion(this->harmonic_motion, count_suspend_time(this->harmonic_motion,
                                                                      get_x_translational_velocity(
                                                                              this->translational_motion)));
}

void update_fish_coordinates(fish_t *this) {
    this->coords->x += get_x_general_velocity(this);
    this->coords->y += get_y_general_velocity(this);
}

void update_fish_dimensions(fish_t *this) {
    this->dimensions.x = this->coords->x;
    this->dimensions.y = this->coords->y;
}




