#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "emulator.h"

bool sdl_init_window(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer);
void sdl_free_window(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer);
void sdl_draw_buffer(SDL_Renderer *sdl_renderer, 
                     bool buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH]);
void sdl_scale_renderer_to_window(SDL_Window *sdl_window, 
                                  SDL_Renderer *sdl_renderer);

#endif
