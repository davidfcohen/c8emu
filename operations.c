#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "decode.h"
#include "emulator.h"
#include "operations.h"

int jump(Emulator *state, int address) {
    if (address < 0 || address >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;
    
    state->pc = address;
    return DECODE_SUCCESS;
}

int jump_offset(Emulator *state, int address) {
    if (address < 0 || address + state->V[0x0] >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;
    
    state->pc = address + state->V[0x0];
    return DECODE_SUCCESS;
}

int sub_call(Emulator *state, int address) {
    if (address < 0 || address >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;
    
    if (!stack_push(state, state->pc))
        return STACK_OVERFLOW;

    state->pc = address;
    return DECODE_SUCCESS;
}

int sub_return(Emulator *state) {
    int address = 0;
    
    if (!stack_pop(state, &address))
        return STACK_EMPTY;

    state->pc = address;
    return DECODE_SUCCESS;
}

int skip_if_register_eq_value(Emulator *state, int X, uint8_t value) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;

    if (state->V[X] == value)
        state->pc += 2;

    return DECODE_SUCCESS;
}

int skip_if_register_ne_value(Emulator *state, int X, uint8_t value) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    if (state->V[X] != value)
        state->pc += 2;

    return DECODE_SUCCESS;
}

int skip_if_register_eq(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    if (state->V[X] == state->V[Y])
        state->pc += 2;

    return DECODE_SUCCESS;
}

int skip_if_register_ne(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;
    
    if (state->V[X] != state->V[Y])
        state->pc += 2;

    return DECODE_SUCCESS;
}

int skip_if_key(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    int key = state->V[X];

    if (key < 0x0 || key > 0xF)
        return INVALID_KEY;

    if (state->keys[key])
        state->pc += 2;
    
    return DECODE_SUCCESS;
}

int skip_if_not_key(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    int key = state->V[X];

    if (key < 0x0 || key > 0xF)
        return INVALID_KEY;

    if (!state->keys[state->V[X]])
        state->pc += 2;
    
    return DECODE_SUCCESS;
}

int get_key(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    int key = state->V[X] & 0xF;

    if (key < 0x0 || key > 0xF)
        return INVALID_KEY;

    if (!state->keys[key])
        state->pc -= 2;

    return DECODE_SUCCESS;
}

int register_set_delay_timer(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;

    state->V[X] = state->delay_timer;
    return DECODE_SUCCESS;
}

int register_set_value(Emulator *state, int X, uint8_t value) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;

    state->V[X] = value;
    return DECODE_SUCCESS;
}

int register_set(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    state->V[X] = state->V[Y];
    return DECODE_SUCCESS;
}

int register_add_value(Emulator *state, int X, uint8_t value) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    state->V[X] += value;
    return DECODE_SUCCESS;
}

int register_add(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    if (state->V[X] + state->V[Y] > UINT8_MAX)
        state->V[0xF] = 1;

    state->V[X] += state->V[Y];
    return DECODE_SUCCESS;
}

int register_sub(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    if (state->V[X] > state->V[Y])
        state->V[0xF] = 1;
    else
        state->V[0xF] = 0;

    state->V[X] -= state->V[Y];
    return DECODE_SUCCESS;
}

int register_shift_right(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    // Original COSMAC VIP behavior:
    // state->V[X] = state->V[Y];
    state->V[0xF] = state->V[X] & 1;

    state->V[X] >>= 1;
    return DECODE_SUCCESS;
}

int register_shift_left(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    // Original COSMAC VIP behavior:
    // state->V[X] = state->V[Y];
    state->V[0xF] = (state->V[X] >> 7) & 1;

    state->V[X] <<= 1;
    return DECODE_SUCCESS;
}

int register_bitwise_or(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    state->V[X] |= state->V[Y];
    return DECODE_SUCCESS;
}

int register_bitwise_and(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    state->V[X] &= state->V[Y];
    return DECODE_SUCCESS;
}

int register_bitwise_xor(Emulator *state, int X, int Y) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;

    state->V[X] ^= state->V[Y];
    return DECODE_SUCCESS;
}

int index_set(Emulator *state, int address) {
    if (address < 0 || address >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;

    state->I = address;
    return DECODE_SUCCESS;
}

int index_add(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;

    if (state->I + state->V[X] >= MEMORY_MAX) // Set VF to 1 if sum is out of bounds.
        state->V[0xF] = 1;                    // Few programs rely on this behavior.

    state->I += state->V[X];
    return DECODE_SUCCESS;
}

int memory_save(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    if (state->I + state->V[X] >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;

    for (int i = 0; i < X + 1; ++i)
        state->memory[state->I + i] = state->V[i];
    return DECODE_SUCCESS;
}

int memory_load(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    if (state->I + state->V[X] >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;

    for (int i = 0; i < X + 1; ++i)
        state->V[i] = state->memory[state->I + i];
    return DECODE_SUCCESS;
}

int timer_delay_set(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    state->delay_timer = state->V[X];
    return DECODE_SUCCESS;
}

int timer_sound_set(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    state->sound_timer = state->V[X];
    return DECODE_SUCCESS;
}

int font_character(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;

    state->I = 5 * (state->V[X] & 0xF);   
    return DECODE_SUCCESS;
}

int hex_to_decimal(Emulator *state, int X) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;
    
    if (state->I + 2 >= MEMORY_MAX)
        return ADDRESS_OUT_OF_BOUNDS;

    int n = state->V[X];
    for (int i = 2; i >= 0; --i) {
        state->memory[state->I + i] = n % 10;
        n /= 10;
    }
    return DECODE_SUCCESS;
}

int random(Emulator *state, int X, uint8_t value) {
    if (X < 0x0 || X > 0xF)
        return INVALID_REGISTER;

    srand((unsigned int) time(NULL));

    state->V[X] = (rand() % UINT8_MAX) & value;
    return DECODE_SUCCESS;
}

int screen_clear(Emulator *state) {
    for (int row = 0; row < DISPLAY_HEIGHT; ++row)
        for (int col = 0; col < DISPLAY_WIDTH; ++col)
            state->display[row][col] = 0;
    return DECODE_SUCCESS;
}

int screen_draw(Emulator *state, int X, int Y, int height) {
    if (X < 0x0 || X > 0xF || Y < 0x0 || Y > 0xF)
        return INVALID_REGISTER;
    
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

    return DECODE_SUCCESS;
}
