#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "emulator.h"
#include "operations.h"

void jump(Emulator *state, int address) {
    if (address >= MEMORY_MAX)
        return;
    state->pc = address;
}

void jump_offset(Emulator *state, int address) {
    if (address + state->V[0x0] >= MEMORY_MAX)
        return;
    state->pc = address + state->V[0x0];
}

void sub_call(Emulator *state, int address) {
    if (!stack_push(state, state->pc))
        return;

    state->pc = address;
}

void sub_return(Emulator *state) {
    int address = 0;
    
    if (!stack_pop(state, &address))
        return;

    state->pc = address;
}

void skip_if_register_eq_value(Emulator *state, int X, uint8_t value) {
    if (state->V[X] == value)
        state->pc += 2;
}

void skip_if_register_ne_value(Emulator *state, int X, uint8_t value) {
    if (state->V[X] != value)
        state->pc += 2;
}

void skip_if_register_eq(Emulator *state, int X, int Y) {
    if (state->V[X] == state->V[Y])
        state->pc += 2;
}

void skip_if_register_ne(Emulator *state, int X, int Y) {
    if (state->V[X] != state->V[Y])
        state->pc += 2;
}

void skip_if_key(Emulator *state, int X) {
    int key = state->V[X];
    
    if (key < 0x0 || key > 0xF)
        return;

    if (state->keys[key])
        state->pc += 2;
}

void skip_if_not_key(Emulator *state, int X) {
    int key = state->V[X];
    
    if (key < 0x0 || key > 0xF)
        return;

    if (!state->keys[key])
        state->pc += 2;
}

void get_key(Emulator *state, int X) {
    int key = state->V[X];

    if (key < 0x0 || key > 0xF)
        return;

    if (!state->keys[key])
        state->pc -= 2;
}

void register_set_delay_timer(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;

    state->V[X] = state->delay_timer;
}

void register_set_value(Emulator *state, int X, uint8_t value) {
    if (X >= REGISTER_MAX)
        return;
    state->V[X] = value;
}

void register_set(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;
    state->V[X] = state->V[Y];
}

void register_add_value(Emulator *state, int X, uint8_t value) {
    if (X >= REGISTER_MAX)
        return;
    state->V[X] += value;
}

void register_add(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    if (state->V[X] + state->V[Y] > UINT8_MAX)
        state->V[0xF] = 1;

    state->V[X] += state->V[Y];
}

void register_sub(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    if (state->V[X] > state->V[Y])
        state->V[0xF] = 1;
    else
        state->V[0xF] = 0;

    state->V[X] -= state->V[Y];
}

void register_shift_right(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    state->V[X] = state->V[Y];
    state->V[0xF] = state->V[X] & 0xF;

    state->V[X] >>= 1;
}

void register_shift_left(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    state->V[X] = state->V[Y];
    state->V[0xF] = state->V[X] & 0xF;

    state->V[X] <<= 1;
}

void register_bitwise_or(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    state->V[X] |= state->V[Y];
}

void register_bitwise_and(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    state->V[X] &= state->V[Y];
}

void register_bitwise_xor(Emulator *state, int X, int Y) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;

    state->V[X] ^= state->V[Y];
}

void index_set(Emulator *state, int address) {
    if (address >= MEMORY_MAX)
        return;
    state->I = address;
}

void index_add(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;

    if (state->I + state->V[X] > 0x0FFF)
        state->V[0xF] = 1;

    state->I += state->V[X];
}

void memory_save(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;
    
    if (state->I + state->V[X] > 0x0FFF)
        return;

    for (int i = 0; i < X + 1; ++i)
        state->memory[state->I + i] = state->V[i];
}

void memory_load(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;
    
    if (state->I + state->V[X] > 0x0FFF)
        return;

    for (int i = 0; i < X + 1; ++i)
        state->V[i] = state->memory[state->I + i];
}

void timer_delay_set(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;
    
    state->delay_timer = state->V[X];
}

void timer_sound_set(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;
    
    state->sound_timer = state->V[X];
}

void hex_to_decimal(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return;
    
    if (state->I + 2 > 0x0FFF)
        return;

    int n = state->V[X];
    for (int i = 2; i >= 0; --i) {
        state->memory[state->I + i] = n % 10;
        n /= 10;
    }
}

void random(Emulator *state, int register_id, uint8_t value) {
    if (register_id >= REGISTER_MAX)
        return;

    srand((unsigned int) time(NULL));
    state->V[register_id] = (rand() % sizeof(uint8_t)) & value;
}

void screen_clear(Emulator *state) {
    for (int row = 0; row < DISPLAY_HEIGHT; ++row)
        for (int col = 0; col < DISPLAY_WIDTH; ++col)
            state->display[row][col] = 0;
}

void screen_draw(Emulator *state, int X, int Y, int height) {
    if (X >= REGISTER_MAX || Y >= REGISTER_MAX)
        return;
    
    int x_init = state->V[X] % DISPLAY_WIDTH;
    int y = state->V[Y] % DISPLAY_HEIGHT;

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