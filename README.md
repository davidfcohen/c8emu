# c8emu
## About
CHIP-8 Emulator written in C using SDL2 for user input and graphics.

## Build

### Linux:
Make sure that `make`, `pkg-config`, and `libsdl2` are installed on your
system. Then, change the current directory to the project folder and run:
```
make c8emu
```

## Usage
```
c8emu [-v] filename
	-v:  Write debug information to stdout.
```

## Controls
```
 CHIP-8 -> QWERTY

1 2 3 C    1 2 3 4
4 5 6 D    Q W E R
7 8 9 E    A S D F
A 0 B F	   Z X C V
```