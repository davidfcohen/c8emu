#include <stdbool.h>
#include <stdint.h>

#include "decode.h"
#include "emulator.h"
#include "instructions.h"

bool decode(Emulator *state, uint16_t instruction) {
    int first_nibble = (instruction >> 12) & 0xF;
    int last_nibble = instruction & 0xF;

    if (first_nibble == 0x0 && last_nibble == 0x0) {
        clear_display(state);

    } else if (first_nibble == 0x0 && last_nibble == 0xE) {
        subroutine_return(state);

    } else if (first_nibble == 0x1) {
        jump(state, NNN(instruction));

    } else if (first_nibble == 0x2) {
        subroutine_call(state, NNN(instruction));

    } else if (first_nibble == 0x3) {
        skip_if_equal(state, state->V[X(instruction)], NN(instruction));

    } else if (first_nibble == 0x4) {
        skip_if_not_equal(state,
                          state->V[X(instruction)], 
                          NN(instruction));

    } else if (first_nibble == 0x5) {
        skip_if_equal(state,
                      state->V[X(instruction)],
                      state->V[Y(instruction)]);

    } else if (first_nibble == 0x6) {
        set_register(state, X(instruction), NN(instruction));

    } else if (first_nibble == 0x7) {
        add_register(state, X(instruction), NN(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x0) {
        set_register(state, 
                     X(instruction), 
                     state->V[Y(instruction)]);

    } else if (first_nibble == 0x8 && last_nibble == 0x1) {
        state->V[X(instruction)] |= state->V[Y(instruction)];

    } else if (first_nibble == 0x8 && last_nibble == 0x2) {
        state->V[X(instruction)] &= state->V[Y(instruction)];

    } else if (first_nibble == 0x8 && last_nibble == 0x3) {
        state->V[X(instruction)] ^= state->V[Y(instruction)];

    } else if (first_nibble == 0x8 && last_nibble == 0x4) {
        add_register_carry(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x5) {
        sub_register(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x6) {
        shift_right(state, X(instruction), Y(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0x7) {
        sub_register(state, Y(instruction), X(instruction));

    } else if (first_nibble == 0x8 && last_nibble == 0xE) {
        shift_left(state, X(instruction), Y(instruction));
    
    } else if (first_nibble == 0x9) {
        skip_if_not_equal(state, 
                          state->V[X(instruction)], 
                          state->V[Y(instruction)]);
       
    } else if (first_nibble == 0xA) {
        set_index(state, NNN(instruction));
    
    } else if (first_nibble == 0xB) {
        jump_with_offset(state, NNN(instruction));
    
    } else if (first_nibble == 0xC) {
        random(state, X(instruction), NN(instruction));
    
    } else if (first_nibble == 0xD) {
        display(state, X(instruction), Y(instruction), N(instruction));
    
    } else {
        return false;
    }

    return true;
}