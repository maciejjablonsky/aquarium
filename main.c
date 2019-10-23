#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "fish.h"
#include "dl_list.h"
#include <stdio.h>


#ifndef bool
typedef enum
{
    false, true
} bool;
#endif

#define FISH_NUMBER 100
#define BOUNCE_RADIUS 10

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

int main(void)
{
    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Rect window_rect = {50, 50, 1080, 720};
    SDL_Init(SDL_INIT_VIDEO);
    if ((window = SDL_CreateWindow("Aquarium", window_rect.x, window_rect.y, window_rect.w, window_rect.h,
                                   SDL_WINDOW_FULLSCREEN_DESKTOP)) == NULL)
    {
        exit(1);
    }
    if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
    {
        exit(1);
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);
    SDL_RenderSetLogicalSize(renderer, window_rect.w, window_rect.h);
    SDL_SetRenderDrawColor(renderer, 102, 204, 255, 255);


    SDL_Texture *background = IMG_LoadTexture(renderer, "background.png");

    SDL_Texture *fish_texture = IMG_LoadTexture(renderer, "pink_fish.png");
    size_t fish_scale = 30;
    SDL_Rect fish_rectangle = {0, 0, (fish_scale * 1.641025641), fish_scale};


    dl_list_t *fish_list = DL_LIST_create(sizeof(fish_t), DL_COPY_POINTER, fish_destructor);
    for (size_t i = 0; i < FISH_NUMBER; ++i)
    {
        DL_LIST_add_item(fish_list, fish_create(window_rect.w - fish_rectangle.w, window_rect.h - fish_rectangle.h));
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
                case SDL_QUIT:quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:quit = true;
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


                    }
                    break;
            }

        }
        SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);
        SDL_RenderSetLogicalSize(renderer, window_rect.w, window_rect.h);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        long double delta = get_delta_time();
        global_delta += delta;
        global_time += delta;
        delta_count++;
        long double average_delta = global_delta / delta_count;

        fps_timer += delta;
        if (fps_timer > 0.5)
        {
            fps = frames * 2;
            frames = 0;
            fps_timer -= 0.5;
        }

        printf("delta = %.12Lf | average delta = %.12Lf | iteration = %lld | fps = %Lf\n", delta, average_delta,
               delta_count, fps);

        double angle = 0;
        for (size_t i = 0; i < DL_LIST_size(fish_list); ++i)
        {
            fish_t *fish = DL_LIST_item_at(fish_list, i);

            if (fish->changing_direction_x == false &&
                (fish->coords.x < BOUNCE_RADIUS || fish->coords.x + fish_rectangle.w > window_rect.w - BOUNCE_RADIUS))
            {
                fish->v.x *= -1;
                fish->changing_direction_x = true;
            }
            else if (fish->changing_direction_x == true && (fish->coords.x >= BOUNCE_RADIUS &&
                                                            fish->coords.x + fish_rectangle.w <=
                                                            window_rect.w - BOUNCE_RADIUS))
            {
                fish->changing_direction_x = false;
            }

            if (fish->changing_direction_y == false &&
                (fish->coords.y < BOUNCE_RADIUS || fish->coords.y + fish_rectangle.h > window_rect.h - BOUNCE_RADIUS))
            {
                fish->v.y *= -1;
                fish->changing_direction_y = true;
            }
            else if (fish->changing_direction_y == true && (fish->coords.y >= BOUNCE_RADIUS &&
                                                            fish->coords.y + fish_rectangle.h <=
                                                            window_rect.h - BOUNCE_RADIUS))
            {
                fish->changing_direction_y = false;
            }

            angle += delta;
            fish->coords.x += fish->v.x * delta;
            fish->coords.y += fish->v.y * delta;
            fish_rectangle.x = (int) fish->coords.x;
            fish_rectangle.y = (int) fish->coords.y;

            fish_display(renderer, fish, fish_texture, &fish_rectangle);
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