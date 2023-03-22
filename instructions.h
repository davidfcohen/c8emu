#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdint.h>

#include "emulator.h"

void clear_display(Emulator *state);

void jump(Emulator *state, uint16_t address);
void jump_with_offset(Emulator *state, uint16_t address);

void subroutine_call(Emulator *state, uint16_t address);
void subroutine_return(Emulator *state);

void skip_if_equal(Emulator *state, uint16_t a, uint16_t b);
void skip_if_not_equal(Emulator *state, uint16_t a, uint16_t b);

void set_register(Emulator *state, int register_id, uint8_t data);

void add_register(Emulator *state, int register_id, uint8_t data);
void add_register_carry(Emulator *state, int x_register_id, int y_register_id);
void sub_register(Emulator *state, int op1_register_id, int op2_register_id);
void shift_right(Emulator *state, int x_register_id, int y_register_id);
void shift_left(Emulator *state, int x_register_id, int y_register_id);

void set_index(Emulator *state, uint16_t address);
void random(Emulator *state, int register_id, uint8_t data);
void display(Emulator *state, int x_register_id, int y_register_id, 
             int height);

#endif