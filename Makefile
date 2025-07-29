# TODO: custom build of Raylib without audio functionality
# TODO: build release

CC := gcc

### DIRECTORIES
RAYLIB_DIR := ./raylib-5.5_win64
RAYLIB_INCLUDE := $(RAYLIB_DIR)/include
RAYLIB_LIB := $(RAYLIB_DIR)/lib

### MAIN PROGRAM VARS
MAIN_CFLAGS := -std=c99 -Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -g -O0
MAIN_INCLUDES := $(RAYLIB_INCLUDE)
MAIN_LIBS := $(RAYLIB_LIB) -lraylib -lwinmm -lgdi32 -lopengl32 -luser32 -lkernel32

### MINIAUDIO VARS
MINIAUDIO_SRC := miniaudio.c

all: build run

run: main.exe
	./main.exe

build: main.c miniaudio.o
	$(CC) $(MAIN_CFLAGS) miniaudio.o -I $(MAIN_INCLUDES) main.c -o main.exe -L $(MAIN_LIBS)

build_miniaudio: miniaudio.c
	$(CC) -c miniaudio.c -o miniaudio.o

clean:
	rm -f *.exe *.o