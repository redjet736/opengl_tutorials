#ifndef _GL_COMMON_H
#define _GL_COMMON_H

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

// glm math libs
#define GLM_FORCE_RADIANS // force glm functions to use radians instead of degrees
#include <glm/glm.hpp>

// supports only a single object
void load_obj(std::string filename, std::vector<glm::vec4> &vertices, 
    std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);

#endif