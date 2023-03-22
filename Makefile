CC ?= gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c17
PKGCONFIG = $(shell which pkg-config)
INCLUDES = $(shell $(PKGCONFIG) --cflags sdl2) 
LIBS = $(shell $(PKGCONFIG) --libs sdl2)

SRC = main.c sdl_wrapper.c emulator.c decode.c instructions.c

all: main

main:
	$(CC) $(SRC) $(CFLAGS) $(INCLUDES) -o main $(LIBS)

clean:
	rm -f main