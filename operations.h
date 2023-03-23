#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

#include "emulator.h"

void clear_screen(Emulator *state);

void jump(Emulator *state, int address);
void jump_offset(Emulator *state, int address);

void sub_call(Emulator *state, int address);
void sub_return(Emulator *state);

void skip_if_register_eq_value(Emulator *state, int X, uint8_t value);
void skip_if_register_ne_value(Emulator *state, int X, uint8_t value);
void skip_if_register_eq(Emulator *state, int X, int Y);
void skip_if_register_ne(Emulator *state, int X, int Y);

void register_set_value(Emulator *state, int X, uint8_t value);
void register_set(Emulator *state, int X, int Y);

void register_add_value(Emulator *state, int X, uint8_t value);
void register_add(Emulator *state, int X, int Y);
void register_sub(Emulator *state, int X, int Y);
void register_shift_right(Emulator *state, int X, int Y);
void register_shift_left(Emulator *state, int X, int Y);

void index_set(Emulator *state, int address);

void random(Emulator *state, int X, uint8_t value);
void display(Emulator *state, int X, int Y, int height);

#endif