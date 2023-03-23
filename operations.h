#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

#include "emulator.h"

void jump(Emulator *state, int address);
void jump_offset(Emulator *state, int address);

void sub_call(Emulator *state, int address);
void sub_return(Emulator *state);

void skip_if_register_eq_value(Emulator *state, int X, uint8_t value);
void skip_if_register_ne_value(Emulator *state, int X, uint8_t value);
void skip_if_register_eq(Emulator *state, int X, int Y);
void skip_if_register_ne(Emulator *state, int X, int Y);

void skip_if_key(Emulator *state, int X);
void skip_if_not_key(Emulator *state, int X);

void get_key(Emulator *state, int X);

void register_set_delay_timer(Emulator *state, int X);
void register_set_value(Emulator *state, int X, uint8_t value);
void register_set(Emulator *state, int X, int Y);

void register_add_value(Emulator *state, int X, uint8_t value);
void register_add(Emulator *state, int X, int Y);
void register_sub(Emulator *state, int X, int Y);

void register_shift_right(Emulator *state, int X, int Y);
void register_shift_left(Emulator *state, int X, int Y);
void register_bitwise_or(Emulator *state, int X, int Y);
void register_bitwise_and(Emulator *state, int X, int Y);
void register_bitwise_xor(Emulator *state, int X, int Y);

void index_set(Emulator *state, int address);
void index_add(Emulator *state, int X);

void memory_save(Emulator *state, int X);
void memory_load(Emulator *state, int X);

void timer_delay_set(Emulator *state, int X);
void timer_sound_set(Emulator *state, int X);

void hex_to_decimal(Emulator *state, int X);
void random(Emulator *state, int X, uint8_t value);

void screen_clear(Emulator *state);
void screen_draw(Emulator *state, int X, int Y, int height);

#endif