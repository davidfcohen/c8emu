#include <stdbool.h>
#include <stdint.h>

#include "decode.h"

bool decode(Emulator *state, uint16_t instruction) {    
    switch (OP(instruction)) {
        case 0x0:
            if (N(instruction) == 0x0)
                clear_display(state);
            else if (N(instruction) == 0xE)
                subroutine_return(state);
            else
                return false;
            break;
        case 0x1:
            jump(state, NNN(instruction));
            break;
        case 0x2:
            subroutine_call(state, NNN(instruction));
            break;
        case 0x3:
            skip_if_equal(state, 
                          state->V[X(instruction)], 
                          NN(instruction));
            break;
        case 0x4:
            skip_if_not_equal(state,
                              state->V[X(instruction)], 
                              NN(instruction));
            break;
        case 0x5:
            skip_if_equal(state,
                          state->V[X(instruction)],
                          state->V[Y(instruction)]);
            break;
        case 0x6:
            set_register(state, X(instruction), NN(instruction));
            break;
        case 0x7:
            add_register(state, X(instruction), NN(instruction));
            break;
        case 0x9:
            skip_if_not_equal(state,
                              state->V[X(instruction)],
                              state->V[Y(instruction)]);
            break;
        case 0xA:
            set_index(state, NNN(instruction));
            break;
        case 0xD: 
            display(state, X(instruction), Y(instruction), N(instruction));
            break;
        default:
            return false;
    } 

    return true;
}