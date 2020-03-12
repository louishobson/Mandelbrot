#!/bin/make

# COMMANDS AND FLAGS

# gcc setup
CC=gcc
CFLAGS=-O3 -std=c99 -Isrc -static

# ar setup
AR=ar
ARFLAGS=-rc



# USEFUL TARGETS

# all
#
# make mandelbrot
all: mandelbrot

# clean
#
# remove all object files and static libraries
.PHONY: clean
clean:
	find . -type f -name "*\.o" -delete -print
	find . -type f -name "*\.a" -delete -print

# revert
#
# revert to source
.PHONY: revert
revert: clean
	rm -fv ./mandelbrot

# install
#
# make a link of the binary in /usr/local/bin
.PHONY: install
install:
	@ ( ln -sfv "$(CURDIR)/mandelbrot" /usr/local/bin/mandelbrot && chmod 755 /usr/local/bin/mandelbrot ) || echo "ERROR: could not create link - do you have permission and have run 'make mandelbrot'?"

# uninstall
#
# remove the link created by the install target
.PHONY: uninstall
	@ rm -fv /usr/local/bin/mandelbrot || echo "ERROR: could not remove link - do you have permission?



# COMPILATION TARGETS

# glad
#
# purely compile glad source
glad: src/glad/glad.o

# glhelper
#
# create static library from glhelper source
glhelper: src/glhelper/glhelper.a
src/glhelper/glhelper.a: src/glhelper/glhelper_input.o src/glhelper/glhelper_draw.o src/glhelper/glhelper_buff.o src/glhelper/glhelper_glsl.o src/glhelper/glhelper_glfw.o src/glhelper/glhelper_glad.o
	$(AR) $(ARFLAGS) $@ $^

# mb_handler
#
# compile with MANDELBROT_INSTALL_PATH defined to the full path
mb_handler: src/mb_handler/mb_handler.o
src/mb_handler/mb_handler.o: src/mb_handler/mb_handler.c
	$(CC) $(CFLAGS) -DMANDELBROT_INSTALL_PATH='"$(CURDIR)"' -c -o $@ $^

# mandelbrot
#
# bring together mandelbrot, mb_handler, glhelper and glad object files to create binary
mandelbrot: mandelbrot.o src/mb_handler/mb_handler.o src/glhelper/glhelper.a src/glad/glad.o
	$(CC) -ldl -lGL -lglfw -lm -o $@ $^



