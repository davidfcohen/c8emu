#include <stdbool.h>
#include <stdint.h>

#include "decode.h"
#include "emulator.h"
#include "instructions.h"

bool decode(Emulator *state, uint16_t instruction) {
    uint16_t operation = instruction & 0xF00F;
    
    switch (operation) {
    case 0x0000:
        clear_display(state);
        break;
    case 0x000E:
        subroutine_return(state);
        break;
    case 0x1000 ... 0x100F:
        jump(state, NNN(instruction));
        break;
    case 0x2000 ... 0x200F:
        subroutine_call(state, NNN(instruction));
        break;
    case 0x3000 ... 0x300F:
        skip_if_equal(state, state->V[X(instruction)], NN(instruction));
        break;
    case 0x4000 ... 0x400F:
        skip_if_not_equal(state,
                          state->V[X(instruction)], 
                          NN(instruction));
        break;
    case 0x5000 ... 0x500F:
        skip_if_equal(state,
                      state->V[X(instruction)],
                      state->V[Y(instruction)]);
        break;
    case 0x6000 ... 0x600F:
        set_register(state, X(instruction), NN(instruction));
        break;
    case 0x7000 ... 0x700F:
        add_register(state, X(instruction), NN(instruction));
        break;
    case 0x8000:
        set_register(state, 
                     X(instruction), 
                     state->V[Y(instruction)]);
        break;
    case 0x8001:
        state->V[X(instruction)] |= state->V[Y(instruction)];
        break;
    case 0x8002:
        state->V[X(instruction)] &= state->V[Y(instruction)];
        break;
    case 0x8003:
        state->V[X(instruction)] ^= state->V[Y(instruction)];
        break;
    case 0x8004:
        add_register_carry(state, X(instruction), Y(instruction));
        break;
    case 0x8005:
        sub_register(state, X(instruction), Y(instruction));
        break;
    case 0x8006:
        shift_right(state, X(instruction), Y(instruction));
        break;
    case 0x8007:
        sub_register(state, Y(instruction), X(instruction));
        break;
    case 0x800E:
        shift_left(state, X(instruction), Y(instruction));
        break;
    case 0x9000 ... 0x900F:
        skip_if_not_equal(state, 
                          state->V[X(instruction)], 
                          state->V[Y(instruction)]);
        break;
    case 0xA000 ... 0xA00F:
        set_index(state, NNN(instruction));
        break;
    case 0xB000 ... 0xB00F:
        jump_with_offset(state, NNN(instruction));
        break;
    case 0xC000 ... 0xC00F: 
        random(state, X(instruction), NN(instruction));
        break;
    case 0xD000 ... 0xD00F:
        display(state, X(instruction), Y(instruction), N(instruction));
        break;
    default:
        return false;
    }

    return true;
}
