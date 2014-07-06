CC=g++
LDLIBS=-lglut -lGLEW -lGL
monkey: shader_utils.o gl_common.o
all: monkey
clean:
	rm -f *.o monkey
.PHONY: all clean