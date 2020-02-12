//
// Created by maciek on 20.09.19.
//

#define _USE_MATH_DEFINES
#include "fish.h"
#include <math.h>
#define to_radians(degrees) ((degrees) * M_PI / 180)
#define to_degrees(radians) ((radians) * 180/ M_PI)

fish_t *fish_create(size_t max_width, size_t max_height, size_t border, SDL_Rect dimensions)
{
    fish_t *new_fish = malloc(sizeof(fish_t));
    if (new_fish == NULL)
    {
        return new_fish;
    }

    new_fish->friends = 0;
    new_fish->coords.x = rand() % (max_width - 2 * border) + border;
    new_fish->coords.y = rand() % (max_height - 2 * border) + border;
    new_fish->v.r = INITIAL_FISH_VELOCITY;
    new_fish->v.x = INITIAL_FISH_VELOCITY * ((long double) rand() / RAND_MAX);
    long double r = new_fish->v.r;
    long double x = new_fish->v.x;
    new_fish->init_angle = to_radians(rand() % 360);
    new_fish->v.y = sqrtl(r * r - x * x);
    new_fish->angle = to_radians(rand() % 360);

//    new_fish->angle = 0;
//    new_fish->coords.x = 100;
//    new_fish->coords.y = 100;

    new_fish->dimensions = dimensions;
    return new_fish;
}


void *fish_kill(fish_t *fish)
{
    free(fish);
    return NULL;
}

void draw_directions(SDL_Renderer * renderer, fish_t *fish, SDL_Rect* fish_dimensions)
{
    double angle = fish->angle;
    int length = 60;
    int x = length * SDL_cos(angle) + fish->coords.x + fish_dimensions->w/2;
    int y = length * SDL_sin(angle) + fish->coords.y + fish_dimensions->h/2;
    SDL_RenderDrawLine(renderer, fish->coords.x + fish_dimensions->w/2, fish->coords.y + fish_dimensions->h/2, x, y);
}

void fish_display(SDL_Renderer *renderer, fish_t *fish, SDL_Texture *fish_texture, SDL_Rect *fish_rectangle, double global_time)
{
    SDL_Point center = {fish_rectangle->w / 2, fish_rectangle->h / 2};
    long double angle = to_degrees(atan2l(fish->v.y, fish->v.x));

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    long double a_angle = fabsl(angle);

    if (90 <= a_angle && a_angle < 180)
    {
        flip = SDL_FLIP_VERTICAL;
    }
    //draw_directions(renderer, fish, fish_rectangle);
    SDL_RenderCopyEx(renderer, fish_texture, NULL, fish_rectangle, angle, &center, flip);
}