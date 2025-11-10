# TODO: custom build of Raylib without audio functionality
# TODO: build release

CC := gcc

### DIRECTORIES
RAYLIB_DIR := ./raylib-5.5_win64
RAYLIB_INCLUDE := $(RAYLIB_DIR)/include
RAYLIB_LIB := $(RAYLIB_DIR)/lib

### MAIN PROGRAM VARS
MAIN_CFLAGS := -std=c99 -Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -O0
MAIN_LIBS := $(RAYLIB_LIB) -lraylib -lwinmm -lgdi32 -lopengl32 -luser32 -lkernel32

### SOURCES
MINIAUDIO_SRC := miniaudio.c

all: build run

run: main.exe
	./main.exe

build: main.c miniaudio.o kissfft-131.2.0/libkissfft-int16_t.a
	$(CC) $(MAIN_CFLAGS) \
	    -Wl,-subsystem,console \
	    -g \
	    miniaudio.o \
	    -I $(RAYLIB_INCLUDE) \
		-I ./kissfft-131.2.0 \
	    main.c \
		-o main.exe \
	    -L $(RAYLIB_LIB) \
		-L ./kissfft-131.2.0 \
	    -lraylib -lwinmm -lgdi32 -lopengl32 -luser32 -lkernel32 -lkissfft-int16_t

kissfft-131.2.0/libkissfft-int16_t.a:
	make -C kissfft-131.2.0 KISSFFT_DATATYPE=int16_t KISSFFT_TOOLS=0 KISSFFT_TEST=0 KISSFFT_STATIC=1 all

miniaudio.o: miniaudio.c
	$(CC) -c -g miniaudio.c -o miniaudio.o


clean:
	rm -f *.exe *.o ./kissfft-131.2.0/*.o