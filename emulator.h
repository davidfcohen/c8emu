#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <stdint.h>

#define SLEEP_TIME_NS(frequency) (1000000000 / frequency)

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

#define MEMORY_MAX 4096
#define STACK_MAX 16

#define PC_START 512

#define FREQUENCY_CPU 700
#define FREQUENCY_TIMER 60

typedef struct emulator {
    int stack[STACK_MAX];
    
    uint8_t V[16];
    uint8_t memory[MEMORY_MAX];
    
    bool keys[16];
    bool display[DISPLAY_HEIGHT][DISPLAY_WIDTH];

    int pc;
    int I;

    int sp;
    int sound_timer;
    int delay_timer;
} Emulator;

int load_font(Emulator *state);
int load_rom(Emulator *state, const char *filename);

uint16_t fetch(Emulator *state);

bool stack_push(Emulator *state, int address);
bool stack_pop(Emulator *state, int *address);

#endif
