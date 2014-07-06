#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>

std::string file_read(const std::string filename);
void print_log(GLuint object);
GLuint create_shader(const std::string filename, GLenum type);

#endif
