#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

#include "emulator.h"

int jump(Emulator *state, int address);
int jump_offset(Emulator *state, int address);

int sub_call(Emulator *state, int address);
int sub_return(Emulator *state);

int skip_if_register_eq_value(Emulator *state, int X, uint8_t value);
int skip_if_register_ne_value(Emulator *state, int X, uint8_t value);
int skip_if_register_eq(Emulator *state, int X, int Y);
int skip_if_register_ne(Emulator *state, int X, int Y);

int skip_if_key(Emulator *state, int X);
int skip_if_not_key(Emulator *state, int X);

int get_key(Emulator *state, int X);

int register_set_delay_timer(Emulator *state, int X);
int register_set_value(Emulator *state, int X, uint8_t value);
int register_set(Emulator *state, int X, int Y);

int register_add_value(Emulator *state, int X, uint8_t value);
int register_add(Emulator *state, int X, int Y);
int register_sub(Emulator *state, int X, int Y);

int register_shift_right(Emulator *state, int X, int Y);
int register_shift_left(Emulator *state, int X, int Y);
int register_bitwise_or(Emulator *state, int X, int Y);
int register_bitwise_and(Emulator *state, int X, int Y);
int register_bitwise_xor(Emulator *state, int X, int Y);

int index_set(Emulator *state, int address);
int index_add(Emulator *state, int X);

int memory_save(Emulator *state, int X);
int memory_load(Emulator *state, int X);

int timer_delay_set(Emulator *state, int X);
int timer_sound_set(Emulator *state, int X);

int font_character(Emulator *state, int X);
int hex_to_decimal(Emulator *state, int X);
int random(Emulator *state, int X, uint8_t value);

int screen_clear(Emulator *state);
int screen_draw(Emulator *state, int X, int Y, int height);

#endif
