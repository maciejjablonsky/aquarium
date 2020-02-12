#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "fish.h"
#include "dl_list.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535
#endif

#ifndef bool
typedef enum
{
    false,
    true
} bool;
#endif

#define FISH_NUMBER 500
#define BOUNCE_RADIUS 10

typedef enum
{
    NONE,
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
} wall_t;

long double velocity_x(fish_t *fish, long double delta_time, long double global_time);
long double velocity_y(fish_t *fish, long double delta_time, long double global_time);

wall_t which_wall_is_going_to_hit(fish_t *fish, long double delta_time, long double global_time, SDL_Rect *aquarium);
bool is_going_to_hit_right_wall(fish_t *fish, long double delta_time, long double global_time, SDL_Rect *aquarium);
bool is_going_to_hit_bottom_wall(fish_t *fish, long double delta_time, long double global_time, SDL_Rect *aquarium);

bool is_going_to_hit_left_wall(fish_t *fish, long double delta_time, long double global_time);

bool is_going_to_hit_top_wall(fish_t *fish, long double delta_time, long double global_time);

void dodge_wall(fish_t *fish, wall_t wall_to_hit);
void dodge_vertical_wall(fish_t *fish);
void dodge_horizontal_wall(fish_t *fish);

long double translational_x(fish_t * fish , long double delta_time);
long double translational_y(fish_t * fish, long double delta_time);

long double harmonic_x(fish_t * fish, long double global_time);
long double harmonic_y(fish_t * fish, long double global_time);

#define to_radians(degrees) ((degrees) * M_PI / 180)
#define to_degrees(radians) ((radians) * 180/ M_PI)


void *fish_destructor(void *item)
{
    fish_kill(item);
    return NULL;
}

long double get_delta_time(void)
{
    static Uint32 t1 = 0;
    Uint32 t2 = SDL_GetTicks();
    long double delta = (t2 - t1) * 0.001;
    t1 = t2;
    return delta;
}

#define AQUARIUM_WIDTH 1080
#define AQUARIUM_HEIGHT 720

int main(void)
{
    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Rect window_rect = {50, 50, AQUARIUM_WIDTH, AQUARIUM_HEIGHT};
    SDL_Init(SDL_INIT_VIDEO);
    if ((window = SDL_CreateWindow("Aquarium", window_rect.x, window_rect.y, window_rect.w, window_rect.h,
                                   SDL_WINDOW_RESIZABLE)) == NULL)
    {
        exit(1);
    }
    if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
    {
        exit(1);
    }
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
   // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);
    SDL_RenderSetLogicalSize(renderer, window_rect.w, window_rect.h);
    SDL_SetRenderDrawColor(renderer, 102, 204, 255, 255);

    SDL_Texture *background = IMG_LoadTexture(renderer, "background.png");

    SDL_Texture *fish_texture = IMG_LoadTexture(renderer, "pink_fish.png");
    size_t fish_scale = FISH_SCALE;
    SDL_Rect fish_rectangle = {0, 0, (fish_scale * 1.641025641), fish_scale};

    dl_list_t *fish_list = DL_LIST_create(sizeof(fish_t), DL_COPY_POINTER, fish_destructor);
    for (size_t i = 0; i < FISH_NUMBER; ++i)
    {
        DL_LIST_add_item(fish_list, fish_create(window_rect.w - fish_rectangle.w, window_rect.h - fish_rectangle.h,
                                                BOUNCE_RADIUS, fish_rectangle));
    }
    bool quit = false;
    long double global_delta = 0;
    long long delta_count = 0;
    long double global_time = 0;
    long double fps_timer, fps;
    fps_timer = 0;
    fps = 0;
    int frames = 0;
    get_delta_time();
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) > 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    for (size_t i = 0; i < FISH_NUMBER; ++i)
                    {
                        fish_t *fish = DL_LIST_item_at(fish_list, i);
#define VELOCITY_FACTOR 1.2
                        fish->v.x *= VELOCITY_FACTOR;
                        fish->v.y *= VELOCITY_FACTOR;
#undef VELOCITY_FACTOR
                    }
                    break;
                case SDLK_DOWN:
                    for (size_t i = 0; i < FISH_NUMBER; ++i)
                    {
                        fish_t *fish = DL_LIST_item_at(fish_list, i);
#define VELOCITY_FACTOR 0.8
                        fish->v.x *= VELOCITY_FACTOR;
                        fish->v.y *= VELOCITY_FACTOR;
#undef VELOCITY_FACTOR
                    }
                    break;
                default:
                    break;
                }
                break;
            }
        }
        SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);
        SDL_RenderSetLogicalSize(renderer, window_rect.w, window_rect.h);

 //       SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        long double delta = get_delta_time();
        global_delta += delta;
        global_time += delta;
        delta_count++;

        fps_timer += delta;
        if (fps_timer > 0.5)
        {
            fps = frames * 2;
            frames = 0;
            fps_timer -= 0.5;
        }

        printf("delta time = %.12Lf |  iteration = %lld | fps = %Lf\n", delta,
               delta_count, fps);
        for (size_t i = 0; i < DL_LIST_size(fish_list); ++i)
        {
            fish_t *fish = DL_LIST_item_at(fish_list, i);
            wall_t wall_to_hit = which_wall_is_going_to_hit(fish, delta, global_time, &window_rect);
            if (wall_to_hit != NONE)
            {
                dodge_wall(fish, wall_to_hit);
            }

            fish->coords.x += velocity_x(fish, delta, global_time);
            fish->coords.y += velocity_y(fish, delta, global_time);

            fish_rectangle.x = (int)fish->coords.x;
            fish_rectangle.y = (int)fish->coords.y;

            fish_display(renderer, fish, fish_texture, &fish_rectangle, global_time);
        }

        SDL_RenderPresent(renderer);

        frames++;
    }

    DL_LIST_delete(fish_list);
    SDL_DestroyTexture(fish_texture);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

wall_t which_wall_is_going_to_hit(fish_t *fish, long double delta_time, long double global_time, SDL_Rect *aquarium)
{
    if (is_going_to_hit_top_wall(fish, delta_time, global_time))
    {
        return TOP;
    }
    else if (is_going_to_hit_right_wall(fish, delta_time, global_time, aquarium))
    {
        return RIGHT;
    }
    else if (is_going_to_hit_bottom_wall(fish, delta_time, global_time, aquarium))
    {
        return BOTTOM;
    }
    else if (is_going_to_hit_left_wall(fish, delta_time, global_time))
    {
        return LEFT;
    }
    return NONE;
}

bool is_going_to_hit_right_wall(fish_t *fish, long double delta_time, long double global_time, SDL_Rect *aquarium)
{
    return fish->coords.x + fish->dimensions.w + velocity_x(fish, delta_time, global_time) >= aquarium->w;
}

bool is_going_to_hit_bottom_wall(fish_t *fish, long double delta_time, long double global_time, SDL_Rect *aquarium)
{
    return fish->coords.y + fish->dimensions.h + velocity_y(fish, delta_time, global_time) >= aquarium->h;
}

bool is_going_to_hit_left_wall(fish_t *fish, long double delta_time, long double global_time)
{
    return fish->coords.x + velocity_x(fish, delta_time, global_time) <= 0;
}

bool is_going_to_hit_top_wall(fish_t *fish, long double delta_time, long double global_time)
{
    return fish->coords.y + velocity_y(fish, delta_time, global_time) <= 0;
}

void dodge_wall(fish_t *fish, wall_t wall_to_hit)
{
    if (wall_to_hit == TOP || wall_to_hit == BOTTOM)
    {
        dodge_horizontal_wall(fish);
    }
    else
    {
        dodge_vertical_wall(fish);
    }
}
void dodge_horizontal_wall(fish_t *fish)
{
    double x = fish->v.r * cos(fish->angle);
    double y = -(fish->v.r * sin(fish->angle));
    fish->angle = atan2(y, x);
}

void dodge_vertical_wall(fish_t *fish)
{
    double x = -(fish->v.r * cos(fish->angle));
    double y = fish->v.r * sin(fish->angle);
    fish->angle = atan2(y, x);
}

long double translational_x(fish_t *fish, long double delta_time)
{
    return fish->v.r * cos(fish->angle) * delta_time;
}

long double translational_y(fish_t *fish, long double delta_time)
{
    return fish->v.r * sin(fish->angle) * delta_time;
}

long double harmonic_x(fish_t * fish, long double global_time)
{
    return FISH_WAVE_AMPLITUDE * FISH_WAVE_ANGULAR_VELOCITY * sin(FISH_WAVE_ANGULAR_VELOCITY * global_time +
    fish->init_angle)  * sin
    (-fish->angle);
}

long double harmonic_y(fish_t * fish, long double global_time)
{
    return FISH_WAVE_AMPLITUDE * FISH_WAVE_ANGULAR_VELOCITY * sin(FISH_WAVE_ANGULAR_VELOCITY * global_time +
    fish->init_angle)  *
    cos(-fish->angle);
}

long double velocity_x(fish_t *fish, long double delta_time, long double global_time)
{
    fish->v.x = translational_x(fish, delta_time) + harmonic_x(fish, global_time);
    return fish->v.x;
}

long double velocity_y(fish_t *fish, long double delta_time, long double global_time)
{
    fish->v.y = translational_y(fish, delta_time) +harmonic_y(fish, global_time);
    return fish->v.y;
}