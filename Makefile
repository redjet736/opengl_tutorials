CC=g++
LDLIBS=-lglut -lGLEW -lGL
cube: shader_utils.o gl_common.o
cube.o: res_texture.c
all: cube
clean:
	rm -f *.o cube
.PHONY: all clean