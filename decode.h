#ifndef DECODE_H
#define DECODE_H

#include <stdbool.h>
#include <stdint.h>

#include "emulator.h"

#define GET_OP(instruction) ((instruction >> 12) & 0xF)
#define GET_X(instruction) ((instruction >> 8) & 0xF)
#define GET_Y(instruction) ((instruction >> 4) & 0xF)
#define GET_N(instruction) (instruction & 0xF)
#define GET_NN(instruction) (instruction & 0xFF)
#define GET_NNN(instruction) (instruction & 0xFFF)

enum decode_status {
    DECODE_SUCCESS,
    DECODE_BAD_INSTRUCTION,
    ADDRESS_OUT_OF_BOUNDS,
    STACK_OVERFLOW,
    STACK_EMPTY,
    INVALID_REGISTER,
    INVALID_KEY,
};

int decode(Emulator *state, uint16_t instruction);

#endif
