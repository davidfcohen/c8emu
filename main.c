#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "decode.h"
#include "emulator.h"

bool sdl_init_window(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer);
void sdl_free_window(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer);
void sdl_draw_buffer(SDL_Renderer *sdl_renderer,
                     bool buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH]);
void sdl_scale_renderer_to_window(SDL_Window *sdl_window, 
                                  SDL_Renderer *sdl_renderer);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("USAGE: %s filename\n", argv[0]);
        return 0;
    }

    Emulator state = {
        .memory = { 0 },
        .display = {{ 0 }},
        .stack = { 0 },
        .V = { 0 },
        .sound_timer = 0,
        .delay_timer = 0,
        .pc = PC_START,
        .sp = 0,
        .I = 0
    };
    
    load_font(&state);
    load_rom(&state, argv[1]);
    
    SDL_Window* sdl_window = NULL;
    SDL_Renderer* sdl_renderer = NULL;
    if(!sdl_init_window(&sdl_window, &sdl_renderer)) {
        return 0;
    }

    SDL_Event e;
    bool sdl_quit = false;
    bool sdl_window_resized;
    
    uint16_t instruction; 
    do { 
        instruction = fetch(&state);
        decode(&state, instruction);

        if (OP(instruction) == 0xD)
            sdl_draw_buffer(sdl_renderer, state.display);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                sdl_quit = true;
            
            sdl_window_resized = (e.type == SDL_WINDOWEVENT &&
                                  e.window.event == SDL_WINDOWEVENT_RESIZED);
            if (sdl_window_resized) {
                sdl_scale_renderer_to_window(sdl_window, sdl_renderer);
                sdl_draw_buffer(sdl_renderer, state.display);
            }
        }

    } while (instruction && !sdl_quit);

    sdl_free_window(&sdl_window, &sdl_renderer);

    return 0;
}

bool sdl_init_window(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL!\n%s", SDL_GetError());
        return false;
    }

    int status = SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, 
                                             DISPLAY_HEIGHT, 
                                             SDL_WINDOW_SHOWN,
                                             sdl_window, 
                                             sdl_renderer);

    if (status < 0) {
        printf("Failed to initialize window and renderer!\n%s", 
               SDL_GetError());
        return false;
    };

    SDL_SetWindowResizable(*sdl_window, SDL_TRUE);
    SDL_SetWindowMinimumSize(*sdl_window, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    return true;
}

void sdl_free_window(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer) {
    SDL_DestroyRenderer(*sdl_renderer);
    SDL_DestroyWindow(*sdl_window);
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