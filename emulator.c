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

void clear_display(Emulator *state) {
    for (int y = 0; y < DISPLAY_HEIGHT; ++y)
        for (int x = 0; x < DISPLAY_WIDTH; ++x)
            state->display[y][x] = 0;
}

void jump(Emulator *state, uint16_t address) {
    if (address >= MEMORY_MAX)
        return;
    state->pc = address;
}

void set_register(Emulator *state, uint8_t register_id, uint16_t data) {
    if (register_id >= REGISTER_MAX)
        return;
    state->V[register_id] = data;
}

void add_register(Emulator *state, uint8_t register_id, uint16_t data) {
    if (register_id >= REGISTER_MAX)
        return;
    state->V[register_id] += data;
}

void set_index(Emulator *state, uint16_t address) {
    state->I = address;
}
