#ifndef DECODE_H
#define DECODE_H

#include <stdbool.h>
#include <stdint.h>

#include "emulator.h"

#define OP(instruction) ((instruction >> 12) & 0xF)
#define X(instruction) ((instruction >> 8) & 0xF)
#define Y(instruction) ((instruction >> 4) & 0xF)
#define N(instruction) (instruction & 0xF)
#define NN(instruction) (instruction & 0xFF)
#define NNN(instruction) (instruction & 0xFFF)

bool decode(Emulator *state, uint16_t instruction);

#endif
