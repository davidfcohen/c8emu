#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "emulator.h"
#include "instructions.h"

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

void jump_with_offset(Emulator *state, uint16_t address) {
    if (address + state->V[0x0] >= MEMORY_MAX)
        return;
    state->pc = address + state->V[0x0];
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

void set_register(Emulator *state, int register_id, uint8_t data) {
    if (register_id >= REGISTER_MAX)
        return;
    state->V[register_id] = data;
}

void add_register(Emulator *state, int register_id, uint8_t data) {
    if (register_id >= REGISTER_MAX)
        return;
    state->V[register_id] += data;
}

void add_register_carry(Emulator *state, int x_register_id, int y_register_id) {
    if (x_register_id >= REGISTER_MAX || y_register_id >= REGISTER_MAX)
        return;

    if (state->V[x_register_id] + state->V[y_register_id] > UINT8_MAX)
        state->V[0xF] = 1;

    state->V[x_register_id] += state->V[y_register_id];
}

void sub_register(Emulator *state, int op1_register_id, int op2_register_id) {
    if (op1_register_id >= REGISTER_MAX || op2_register_id >= REGISTER_MAX)
        return;

    if (state->V[op1_register_id] > state->V[op2_register_id])
        state->V[0xF] = 1;
    else
        state->V[0xF] = 0;

    state->V[op1_register_id] -= state->V[op2_register_id];
}

void shift_right(Emulator *state, int x_register_id, int y_register_id) {
    if (x_register_id >= REGISTER_MAX || y_register_id >= REGISTER_MAX)
        return;

    state->V[x_register_id] = state->V[y_register_id];
    state->V[0xF] = state->V[x_register_id] & 0xF;

    state->V[x_register_id] >>= 1;
}

void shift_left(Emulator *state, int x_register_id, int y_register_id) {
    if (x_register_id >= REGISTER_MAX || y_register_id >= REGISTER_MAX)
        return;

    state->V[x_register_id] = state->V[y_register_id];
    state->V[0xF] = (state->V[x_register_id] >> 12) & 0xF;

    state->V[x_register_id] <<= 1;
}

void set_index(Emulator *state, uint16_t address) {
    if (address >= MEMORY_MAX)
        return;
    state->I = address;
}

void random(Emulator *state, int register_id, uint8_t data) {
    if (register_id >= REGISTER_MAX)
        return;

    srand((unsigned int) time(NULL));
    state->V[register_id] = (rand() % sizeof(uint8_t)) & data;
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