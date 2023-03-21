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

void subroutine_call(Emulator *state, uint16_t address) {
    if (address >= MEMORY_MAX || !stack_push(state, state->pc))
        return;
    state->pc = address;
}

void subroutine_return(Emulator *state) {
    uint16_t address = 0;
    if (!stack_pop(state, &address))
        return;
    state->pc = address;
}

void skip_if_equal(Emulator *state, uint16_t a, uint16_t b) {
    if (a == b)
        state->pc += 2;
}

void skip_if_not_equal(Emulator *state, uint16_t a, uint16_t b)     {
    if (a != b)
        state->pc += 2;
}

void set_register(Emulator *state, int register_id, uint16_t data) {
    if (register_id >= REGISTER_MAX)
        return;
    state->V[register_id] = data;
}

void add_register(Emulator *state, int register_id, uint16_t data) {
    if (register_id >= REGISTER_MAX)
        return;
    state->V[register_id] += data;
}

void set_index(Emulator *state, uint16_t address) {
    if (address >= MEMORY_MAX)
        return;
    state->I = address;
}

void display(Emulator *state, int x_register_id, int y_register_id,
             int height) {
    
    if (x_register_id >= REGISTER_MAX || y_register_id >= REGISTER_MAX)
        return;
    
    int x_init = state->V[x_register_id] % DISPLAY_WIDTH;
    int y = state->V[y_register_id] % DISPLAY_HEIGHT;

    state->V[0xF] = 0;

    uint16_t sprite_row_address;
    uint8_t sprite_row;

    bool sprite_pixel_is_on;
    bool display_pixel_is_on;

    int x;
    for (int row = 0; row < height; ++row) {
        sprite_row_address = state->I + row;
        sprite_row = state->memory[sprite_row_address];

        x = x_init;
        for (int pixel = 8; pixel > 0; --pixel) {
            sprite_pixel_is_on = (sprite_row >> (pixel - 1)) & 1;
            display_pixel_is_on = state->display[y][x];
            
            if (sprite_pixel_is_on && display_pixel_is_on) {
                state->display[y][x] = 0;
                state->V[0xF] = 1;
            } else if (sprite_pixel_is_on && !display_pixel_is_on) {
                state->display[y][x] = 1;
            }

            if (++x == DISPLAY_WIDTH) {
                break;
            }
        }

        if (++y == DISPLAY_HEIGHT) {
            break;
        }
    }

    return;
}

bool stack_push(Emulator *state, uint16_t address) {
    if (state->sp >= STACK_MAX - 1)
        return false;
    
    state->stack[state->sp++] = address;
    return true;
}

bool stack_pop(Emulator *state, uint16_t *address) {
    if (state->sp == 0)
        return false;
        
    *address = state->stack[--state->sp];
    return true;
}
