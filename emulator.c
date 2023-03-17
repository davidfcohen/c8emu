#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "emulator.h"

void init(struct chip_8 *state, const char *filename) {
    uint8_t font_data[] = {
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

    int font_data_size = sizeof(font_data) / sizeof(font_data[0]);
    for (int p = FONT_START; 
         p < FONT_START + font_data_size && p < PC_START; 
         p++) {
        state->memory[p] = font_data[p - FONT_START];
    }

    load_rom(state, filename);
}

void fetch(struct chip_8 *state) {
    return;
}

void decode(struct chip_8 *state) {
    return;
}

void execute(struct chip_8 *state) {
    return;
}

int load_rom(struct chip_8 *state, const char *filename) {
    FILE *rom = fopen(filename, "r");

    if (rom == NULL)
        return -1;

    int n_read = fread(state->memory + PC_START,
                       sizeof(uint8_t),
                       MEMORY_MAX - PC_START,
                       rom);

    fclose(rom);
    return n_read;
}

void display_clear(struct chip_8 *state) {
    for (int y = 0; y < DISPLAY_HEIGHT; ++y)
        for (int x = 0; x < DISPLAY_WIDTH; ++x)
            state->display[y][x] = 0;
}

bool stack_push(struct chip_8 *state, int address) {
    if (state->sp >= STACK_MAX) {
        printf("CHIP-8: Stack Overflow\n");
        return false;
    }

    state->stack[state->sp++] = address;
    return true;
}

int stack_pop(struct chip_8 *state) {
    if (state->sp <= 0) {
        printf("CHIP-8: Stack Empty\n");
        return -1;
    }
    
    return state->stack[--state->sp];
}
