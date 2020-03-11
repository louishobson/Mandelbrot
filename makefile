#!/bin/make

# gcc setup
CC=gcc
CFLAGS=-O3 -std=c99 -D_DEFAULT_SOURCE -fPIC -Isrc

# ar setup
AR=ar
ARFLAGS=-rc

# make all
all: glad



# clean
.PHONY: clean
clean:
	find . -type f -name "*\.o" -delete -print
	find . -type f -name "*\.a" -delete -print



# glad
glad: src/glad/glad.o

# glhelper
glhelper: src/glhelper/glhelper.a
src/glhelper/glhelper.a: src/glhelper/glhelper_input.o src/glhelper/glhelper_draw.o src/glhelper/glhelper_buff.o src/glhelper/glhelper_glsl.o src/glhelper/glhelper_glfw.o src/glhelper/glhelper_glad.o
	$(AR) $(ARFLAGS) $@ $^

# mb_handler
mb_handler: src/mb_handler/mb_handler.o

# mandelbrot
mandelbrot: mandelbrot.o src/mb_handler/mb_handler.o src/glhelper/glhelper.a src/glad/glad.o
	$(CC) -ldl -lGL -lglfw -lm -o $@ $^



