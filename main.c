#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "decode.h"
#include "emulator.h"
#include "sdl_wrapper.h"

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
    
    struct timespec sleep_timespec = {
        .tv_sec = 0,
        .tv_nsec = SLEEP_TIME_NS(FREQUENCY)
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

        nanosleep(&sleep_timespec, NULL);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                sdl_quit = true;
            
            sdl_window_resized = (e.type == SDL_WINDOWEVENT &&
                                  e.window.event == SDL_WINDOWEVENT_RESIZED);
            if (sdl_window_resized) {
                sdl_scale_renderer_to_window(sdl_window, sdl_renderer);
                sdl_draw_buffer(sdl_renderer, state.display);
                sdl_window_resized = false;
            }
        }

    } while (instruction && !sdl_quit);

    sdl_free_window(sdl_window, sdl_renderer);

    return 0;
}
