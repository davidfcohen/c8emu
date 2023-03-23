#include <stdbool.h>
#include <stdint.h>

#include "decode.h"
#include "emulator.h"
#include "operations.h"

int decode(Emulator *state, uint16_t instruction) {
    if (instruction == 0x0000) {
        return DECODE_SUCCESS;

    } else if (instruction == 0x00E0) {
        screen_clear(state);

    } else if (instruction == 0x00EE) {
        sub_return(state);

    } else if (OP(instruction) == 0x1) {
        jump(state, NNN(instruction));

    } else if (OP(instruction) == 0x2) {
        sub_call(state, NNN(instruction));

    } else if (OP(instruction) == 0x3) {
        skip_if_register_eq_value(state, X(instruction), NN(instruction));

    } else if (OP(instruction) == 0x4) {
        skip_if_register_ne_value(state, X(instruction), NN(instruction));

    } else if (OP(instruction) == 0x5) {
        skip_if_register_eq(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x6) {
        register_set_value(state, X(instruction), NN(instruction));

    } else if (OP(instruction) == 0x7) {
        register_add_value(state, X(instruction), NN(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x0) {
        register_set(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x1) {
        register_bitwise_or(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x2) {
        register_bitwise_or(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x3) {
        register_bitwise_xor(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x4) {
        register_add(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x5) {
        register_sub(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x6) {
        register_shift_right(state, X(instruction), Y(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0x7) {
        register_sub(state, Y(instruction), X(instruction));

    } else if (OP(instruction) == 0x8 && N(instruction) == 0xE) {
        register_shift_left(state, X(instruction), Y(instruction));
    
    } else if (OP(instruction) == 0x9) {
        skip_if_register_ne(state, X(instruction), Y(instruction));
       
    } else if (OP(instruction) == 0xA) {
        index_set(state, NNN(instruction));
    
    } else if (OP(instruction) == 0xB) {
        jump_offset(state, NNN(instruction));
    
    } else if (OP(instruction) == 0xC) {
        random(state, X(instruction), NN(instruction));
    
    } else if (OP(instruction) == 0xD) {
        screen_draw(state, X(instruction), Y(instruction), N(instruction));
    
    } else if (OP(instruction) == 0xE && NN(instruction) == 0x9E) {
        skip_if_key(state, X(instruction));

    } else if (OP(instruction) == 0xE && NN(instruction) == 0xA1) {
        skip_if_not_key(state, X(instruction));

    } else if (OP(instruction) == 0xF && NN(instruction) == 0x07) {
        register_set_delay_timer(state, X(instruction));

    } else if (OP(instruction) == 0xF && NN(instruction) == 0x0A) {
        get_key(state, X(instruction));
    
    } else if (OP(instruction) == 0xF && NN(instruction) == 0x15) {
        timer_delay_set(state, X(instruction));

    } else if (OP(instruction) == 0xF && NN(instruction) == 0x18) {
        timer_sound_set(state, X(instruction));

    } else if (OP(instruction) == 0xF && NN(instruction) == 0x1E) {
        index_add(state, X(instruction));
    
    } else if (OP(instruction) == 0xF && NN(instruction) == 0x33) {
        hex_to_decimal(state, X(instruction));

    } else if (OP(instruction) == 0xF && NN(instruction) == 0x55) {
        memory_save(state, X(instruction));

    } else if (OP(instruction) == 0xF && NN(instruction) == 0x65) {
        memory_load(state, X(instruction));

    } else {
        return DECODE_BAD_INSTRUCTION;
    }   

    return DECODE_SUCCESS;
}