#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <stdint.h>

#define SLEEP_TIME_NS(frequency) (1000000000 / frequency)

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

#define MEMORY_MAX 4096
#define STACK_MAX 16
#define REGISTER_MAX 16

#define PC_START 512

#define FREQUENCY 700

typedef struct emulator {
    uint16_t stack[STACK_MAX];
    uint8_t V[REGISTER_MAX];
    uint8_t memory[MEMORY_MAX];
    
    bool display[DISPLAY_HEIGHT][DISPLAY_WIDTH];

    uint16_t pc;
    uint16_t I;

    uint8_t sp;
    uint8_t sound_timer;
    uint8_t delay_timer;
} Emulator;

int load_font(Emulator *state);
int load_rom(Emulator *state, const char *filename);

uint16_t fetch(Emulator *state);

bool stack_push(Emulator *state, uint16_t address);
bool stack_pop(Emulator *state, uint16_t *address);

#endif
