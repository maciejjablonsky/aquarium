//
// Created by maciek on 20.09.19.
//

//#define DEBUG
#ifndef AQUARIUM_FISH_H
#define AQUARIUM_FISH_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct
{
    long double x;
    long double y;
    long double r;
} velocity_t;

typedef struct
{
    long double x;
    long double y;
} point_t;


typedef struct fish
{
    point_t coords;
    velocity_t v;
    bool changing_direction_x;
    bool changing_direction_y;
    size_t friends;
} fish_t;

#define MIN_FISH_TO_FISH_DISTANCE 30 // e.g. cm
#define MAX_FRIENDS_NUMBER 6
#define FISH_REACTION_RADIUS 100
#define INITIAL_FISH_VELOCITY 200

fish_t *fish_create(size_t max_width, size_t max_height, size_t border);

void *fish_kill(fish_t *fish);

void fish_display(SDL_Renderer *renderer, fish_t *fish, SDL_Texture *fish_texture, SDL_Rect *fish_rectangle);

#endif //AQUARIUM_FISH_H
