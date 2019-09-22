//
// Created by maciek on 20.09.19.
//

#include "fish.h"
#include <math.h>


fish_t *fish_create(size_t max_width, size_t max_height)
{
    fish_t *new_fish = malloc(sizeof(fish_t));
    if (new_fish == NULL)
    {
        return new_fish;
    }

    new_fish->friends = 0;

    new_fish->coords.x = rand() % max_width;
    new_fish->coords.y = rand() % max_height;
    new_fish->v.r = INITIAL_FISH_VELOCITY;
    new_fish->v.x = INITIAL_FISH_VELOCITY * ((double) rand() / RAND_MAX);
    long double r = new_fish->v.r;
    long double x = new_fish->v.x;
    new_fish->v.y = sqrtl(r * r - x * x);

    if (rand() % 2)
    {
        new_fish->v.y *= -1;
    }
    if (rand() % 2)
    {
        new_fish->v.x *= -1;
    }

    new_fish->changing_direction_x = new_fish->changing_direction_y = false;
    return new_fish;
}

void *fish_kill(fish_t *fish)
{
    free(fish);
}

void fish_display(SDL_Renderer *renderer, fish_t *fish, SDL_Texture *fish_texture, SDL_Rect *fish_rectangle)
{
    SDL_Point center = {fish_rectangle->w / 2, fish_rectangle->h / 2};
    long double angle = atan2l(fish->v.y, fish->v.x) * 180 / M_PI;


    SDL_RendererFlip flip = SDL_FLIP_NONE;
    long double a_angle = fabsl(angle);

    if (0 <= a_angle && a_angle < 90)
    {
        flip = SDL_FLIP_NONE;
    }
    else if (90 <= a_angle && a_angle < 180)
    {
        flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(renderer, fish_texture, NULL, fish_rectangle, (double) angle, &center, flip);
}