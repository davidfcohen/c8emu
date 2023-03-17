#ifndef EMULATOR_H
#define EMULATOR_H

#define MEMORY_MAX 4096
#define STACK_MAX 16
#define PC_START 512

#define FONT_START 0x50

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

struct chip_8 {
    uint8_t memory[MEMORY_MAX];
    bool display[DISPLAY_HEIGHT][DISPLAY_WIDTH];
    int stack[STACK_MAX];

    uint8_t sound_timer;
    uint8_t delay_timer;

    int pc;
    int sp;
};

void init(struct chip_8 *state, const char *filename);
void fetch(struct chip_8 *state);
void decode(struct chip_8 *state);
void execute(struct chip_8 *state);

int load_rom(struct chip_8 *state, const char *filename);

void display_clear(struct chip_8 *state);

bool stack_push(struct chip_8 *state, int address);
int stack_pop(struct chip_8 *state);

#endif