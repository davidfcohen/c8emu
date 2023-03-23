#include <stdbool.h>
#include <stdint.h>

#include "decode.h"
#include "emulator.h"
#include "operations.h"

bool decode(Emulator *state, uint16_t instruction) {
    int first_nibble = (instruction >> 12) & 0xF;
    int last_nibble = instruction & 0xF;

    if (instruction == 0x00E0) {
        clear_screen(state);

    } else if (instruction == 0x00EE) {
        sub_return(state);

    } else if (first_nibble == 0x1) {
        jump(state, NNN(instruction));

    } else if (first_nibble == 0x2) {
        sub_call(state, NNN(instruction));

    } else if (first_nibble == 0x3) {
        skip_if_register_eq_value(state, X(instruction), NN(instruction));

    } else if (first_nibble == 0x4) {
        skip_if_register_ne_value(state, X(instruction), NN(instruction));

    } else if (first_nibble == 0x5) {
        skip_if_register_eq(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x6) {
        register_set_value(state, X(instruction), NN(instruction));

    } else if (first_nibble == 0x7) {
        register_add_value(state, X(instruction), NN(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x0) {
        register_set(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x1) {
        state->V[X(instruction)] |= state->V[Y(instruction)];

    } else if (first_nibble == 0x8 && last_nibble == 0x2) {
        state->V[X(instruction)] &= state->V[Y(instruction)];

    } else if (first_nibble == 0x8 && last_nibble == 0x3) {
        state->V[X(instruction)] ^= state->V[Y(instruction)];

    } else if (first_nibble == 0x8 && last_nibble == 0x4) {
        register_add(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x5) {
        register_sub(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x6) {
        register_shift_right(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x7) {
        register_sub(state, Y(instruction), X(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0xE) {
        register_shift_left(state, X(instruction), Y(instruction));
    
    } else if (first_nibble == 0x9) {
        skip_if_register_ne(state, X(instruction), Y(instruction));
       
    } else if (first_nibble == 0xA) {
        index_set(state, NNN(instruction));
    
    } else if (first_nibble == 0xB) {
        jump_offset(state, NNN(instruction));
    
    } else if (first_nibble == 0xC) {
        random(state, X(instruction), NN(instruction));
    
    } else if (first_nibble == 0xD) {
        display(state, X(instruction), Y(instruction), N(instruction));
    
    } else {
        return false;
    }

    return true;
}