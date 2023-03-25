CC ?= gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c17
PKGCONFIG = $(shell which pkg-config)
INCLUDES = $(shell $(PKGCONFIG) --cflags sdl2) 
LIBS = $(shell $(PKGCONFIG) --libs sdl2)

SRC = main.c sdl_wrapper.c emulator.c decode.c operations.c

all: c8emu

c8emu:
	$(CC) $(SRC) $(CFLAGS) $(INCLUDES) -o c8emu $(LIBS)

clean:
	rm -f c8emu
