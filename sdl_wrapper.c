#include <stdbool.h>
#include <SDL2/SDL.h>

#include "emulator.h"
#include "sdl_wrapper.h"

bool sdl_init_window(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;

    int status = SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, 
                                             DISPLAY_HEIGHT, 
                                             SDL_WINDOW_SHOWN,
                                             sdl_window, 
                                             sdl_renderer);

    if (status < 0)
        return false;

    SDL_SetWindowResizable(*sdl_window, SDL_TRUE);
    SDL_SetWindowMinimumSize(*sdl_window, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    return true;
}

void sdl_free_window(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer) {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void sdl_draw_buffer(SDL_Renderer *sdl_renderer, 
                     bool buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH]) {
    for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
        for (int x = 0; x < DISPLAY_WIDTH; ++x) {
            if (buffer[y][x])
                SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            else
                SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderDrawPoint(sdl_renderer, x, y);
        }
    }

    SDL_RenderPresent(sdl_renderer);
}

void sdl_scale_renderer_to_window(SDL_Window *sdl_window, 
                                  SDL_Renderer *sdl_renderer) {
    int window_width, window_height;
    SDL_GetWindowSize(sdl_window, &window_width, &window_height);

    SDL_RenderSetScale(sdl_renderer, 
                       (float) window_width /  DISPLAY_WIDTH,
                       (float) window_height / DISPLAY_HEIGHT);
}
