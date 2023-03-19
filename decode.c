#include <stdbool.h>
#include <stdint.h>

#include "decode.h"

bool decode(Emulator *state, uint16_t instruction) {    
    switch (OP(instruction)) {
        case 0x0:
            clear_display(state);
            break;
        case 0x1:
            jump(state, NNN(instruction));
            break;
        case 0x6:
            set_register(state, X(instruction), NN(instruction));
            break;
        case 0x7:
            add_register(state, X(instruction), NN(instruction));
            break;
        case 0xA:
            set_index(state, NNN(instruction));
            break;
        case 0xD: 
        default:
            return false;
    } 

    return true;
}