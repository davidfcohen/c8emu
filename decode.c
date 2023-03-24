#include <stdbool.h>
#include <stdint.h>

#include "decode.h"
#include "emulator.h"
#include "operations.h"

int decode(Emulator *state, uint16_t instruction) {
    int OP = GET_OP(instruction);
    int X = GET_X(instruction);
    int Y = GET_Y(instruction);
    int N = GET_N(instruction);
    int NN = GET_NN(instruction);
    int NNN = GET_NNN(instruction);

    if (instruction == 0x0000)
        return DECODE_SUCCESS;

    if (instruction == 0x00E0)
        return screen_clear(state);

    if (instruction == 0x00EE)
        return sub_return(state);

    if (OP == 0x1)
        return jump(state, NNN);

    if (OP == 0x2)
        return sub_call(state, NNN);

    if (OP == 0x3)
        return skip_if_register_eq_value(state, X, NN);

    if (OP == 0x4)
        return skip_if_register_ne_value(state, X, NN);

    if (OP == 0x5)
        return skip_if_register_eq(state, X, Y);

    if (OP == 0x6)
        return register_set_value(state, X, NN);

    if (OP == 0x7)
        return register_add_value(state, X, NN);

    if (OP == 0x8 && N == 0x0)
        return register_set(state, X, Y);

    if (OP == 0x8 && N == 0x1)
        return register_bitwise_or(state, X, Y);

    if (OP == 0x8 && N == 0x2)
        return register_bitwise_and(state, X, Y);

    if (OP == 0x8 && N == 0x3)
        return register_bitwise_xor(state, X, Y);

    if (OP == 0x8 && N == 0x4)
        return register_add(state, X, Y);

    if (OP == 0x8 && N == 0x5)
        return register_sub(state, X, Y);

    if (OP == 0x8 && N == 0x6)
        return register_shift_right(state, X, Y);

    if (OP == 0x8 && N == 0x7)
        return register_sub(state, Y, X);

    if (OP == 0x8 && N == 0xE)
        return register_shift_left(state, X, Y);
    
    if (OP == 0x9)
        return skip_if_register_ne(state, X, Y);
       
    if (OP == 0xA)
        return index_set(state, NNN);
    
    if (OP == 0xB)
        return jump_offset(state, NNN);
    
    if (OP == 0xC)
        return random(state, X, NN);
    
    if (OP == 0xD)
        return screen_draw(state, X, Y, N);
    
    if (OP == 0xE && NN == 0x9E)
        return skip_if_key(state, X);

    if (OP == 0xE && NN == 0xA1)
        return skip_if_not_key(state, X);

    if (OP == 0xF && NN == 0x07)
        return register_set_delay_timer(state, X);

    if (OP == 0xF && NN == 0x0A)
        return get_key(state, X);
    
    if (OP == 0xF && NN == 0x15)
        return timer_delay_set(state, X);

    if (OP == 0xF && NN == 0x18)
        return timer_sound_set(state, X);

    if (OP == 0xF && NN == 0x1E)
        return index_add(state, X);

    if (OP == 0xF && NN == 0x29)
        return font_character(state, X);
    
    if (OP == 0xF && NN == 0x33)
        return hex_to_decimal(state, X);

    if (OP == 0xF && NN == 0x55)
        return memory_save(state, X);

    if (OP == 0xF && NN == 0x65)
        return memory_load(state, X);

    return DECODE_BAD_INSTRUCTION;
}
