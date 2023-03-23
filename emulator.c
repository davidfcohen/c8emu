#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "emulator.h"

int load_font(Emulator *state) {
    uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    int font_size = sizeof(font) / sizeof(font[0]);
    
    int i;
    for (i = 0; i < font_size && i < PC_START; ++i) {
        state->memory[i] = font[i];
    }

    return i;
}

int load_rom(Emulator *state, const char *filename) {
    FILE *rom = fopen(filename, "r");

    if (rom == NULL)
        return -1;

    int n = fread(state->memory + PC_START, 1, MEMORY_MAX - PC_START, rom);

    fclose(rom);
    return n;
}

uint16_t fetch(Emulator *state) {    
    uint16_t instruction;

    instruction = state->memory[state->pc] << 8;
    instruction += state->memory[state->pc + 1];

    if (state->pc < MEMORY_MAX - 2)
        state->pc += 2;

    return instruction;
}

bool stack_push(Emulator *state, int address) {
    if (state->sp >= STACK_MAX - 1)
        return false;
    
    state->stack[state->sp++] = address;
    return true;
}

bool stack_pop(Emulator *state, int *address) {
    if (state->sp == 0)
        return false;
        
    *address = state->stack[--state->sp];
    return true;
}
