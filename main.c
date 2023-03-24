#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "decode.h"
#include "emulator.h"
#include "sdl_wrapper.h"

void print_verbose_output(Emulator *state, uint16_t instruction);
void print_decode_error(int decode_error);

int main(int argc, char *argv[]) {
    Emulator state = {
        .stack = { 0 },
        .V = { 0 },
        .memory = { 0 },
        .keys = { 0 },
        .display = {{ 0 }},
        .sound_timer = 0,
        .delay_timer = 0,
        .pc = PC_START,
        .sp = STACK_MAX,
        .I = 0
    };
    
    struct timespec sleep_timespec = {
        .tv_sec = 0,
        .tv_nsec = SLEEP_TIME_NS(FREQUENCY_CPU)
    };

    bool verbose = false;

    if (argc < 2) {
        printf("%s [-v] file\n"
               "\t-v: Write debug information to stdout.\n", argv[0]);
        return 0;
    }

    int i;
    for (i = 1; i < argc - 1; i++)
        if (strcmp(argv[i], "-v") == 0)
            verbose = true;

    load_font(&state);
    load_rom(&state, argv[i]);
    
    SDL_Window* sdl_window = NULL;
    SDL_Renderer* sdl_renderer = NULL;
    if(!sdl_init_window(&sdl_window, &sdl_renderer)) {
        return 0;
    }

    SDL_Event e;
    bool sdl_quit = false;
    bool sdl_window_resized;
    
    int decode_error;

    uint16_t prev = 0;
    uint16_t instruction = 0;

    int delay_timing = (int) round((double) FREQUENCY_CPU / FREQUENCY_TIMER);
    int delay_count = 0;

    do { 
        instruction = fetch(&state);

        if (++delay_count == delay_timing) {
            if (state.delay_timer)
                --state.delay_timer;
            if (state.sound_timer)
                --state.sound_timer;
            delay_count = 0;
        }

        decode_error = decode(&state, instruction);
        if (verbose && instruction != prev) {
            print_verbose_output(&state, instruction);
        }
        prev = instruction;

        if (decode_error) {
            print_decode_error(decode_error);
            break;
        }

        if (GET_OP(instruction) == 0xD)
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

void print_verbose_output(Emulator *state, uint16_t instruction) {
    printf("INSTRUCTION=%04x PC=%04x I=%04x SP=%01x "
           "SOUND=%02x DELAY=%02x\n\t"
           "V0=%02x V1=%02x V2=%02x V3=%02x "
           "V4=%02x V5=%02x V6=%02x V7=%02x\n\t"
           "V8=%02x V9=%02x VA=%02x VB=%02x "
           "VC=%02x VD=%02x VE=%02x VF=%02x\n\n",
           instruction, state->pc, state->I, state->sp,
           state->sound_timer, state->delay_timer,
           state->V[0x0], state->V[0x1], state->V[0x2], state->V[0x3],
           state->V[0x4], state->V[0x5], state->V[0x6], state->V[0x7],
           state->V[0x8], state->V[0x9], state->V[0xA], state->V[0xB],
           state->V[0xC], state->V[0xD], state->V[0xE], state->V[0xF]);
}

void print_decode_error(int decode_error) {
    switch(decode_error) {
        case DECODE_BAD_INSTRUCTION:
            printf("ERROR: DECODE_BAD_INSTRUCTION\n");
            break;
        case ADDRESS_OUT_OF_BOUNDS:
            printf("ERROR: ADDRESS_OUT_OF_BOUNDS\n");
            break;
        case STACK_OVERFLOW:
            printf("ERROR: STACK_OVERFLOW\n");
            break;
        case STACK_EMPTY:
            printf("ERROR: STACK_EMPTY\n");
            break;
        case INVALID_REGISTER:
            printf("ERROR: INVALID_REGISTER\n");
            break;
        case INVALID_KEY:
            printf("ERROR: INVALID_KEY\n");
            break;
        default:
            printf("ERROR: UNKNOWN_ERROR\n");
    }
}
