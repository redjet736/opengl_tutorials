#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

 // Use glew.h instead of gl.h to get all the GL prototypes declared 
#include <GL/glew.h>

 // Using the GLUT library for the base windowing setup 
#include <GL/freeglut.h>

// glm math libs
#define GLM_FORCE_RADIANS // force glm functions to use radians instead of degrees
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_utils.h"
#include "gl_common.h"
#include "res_texture.c"

using namespace std;

 // GLOBAL VARIABLES 
GLuint program;
GLuint vbo_mesh_vertices;//, vbo_mesh_normals;
GLuint ibo_mesh_elements;

GLint uniform_mvp;
const string mvp_name = "mvp";
GLint attribute_v_coord;
const string v_coord_name = "v_coord";
// GLint attribute_v_normal;
// const string v_normal_name = "v_normal";

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// CONSTANTS
const string VS_FILENAME = "monkey.v.glsl";
const string FS_FILENAME = "monkey.f.glsl";
const string MESH_FILENAME = "suzanne.obj";
const int SCREEN_X = 600;
const int SCREEN_Y = 300;
const string TITLE = "Monkey";

// STRUCTS

/*
Function: init_resources
Receives: void
Returns: int
This function creates all GLSL related stuff
explained in this example.
Returns 1 when all is ok, 0 with a displayed error
*/
int init_resources(void)
{    
    // ----- LOAD DATA ------
    vector<glm::vec4> vertices;
    vector<glm::vec3> normals;
    vector<GLushort> elements;

    load_obj(MESH_FILENAME, vertices, normals, elements);

    glGenBuffers(1, &vbo_mesh_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    // glGenBuffers(1, &vbo_mesh_normals);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
    // glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_mesh_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(elements[0]), elements.data(), GL_STATIC_DRAW);

    // ----- CREATE SHADERS ------
    GLuint vs, fs;
    vs = create_shader(VS_FILENAME, GL_VERTEX_SHADER);
    if (0 == vs) return 0;

    fs = create_shader(FS_FILENAME, GL_FRAGMENT_SHADER);
    if (0 == fs) return 0;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    GLint link_ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) 
    {
        cerr << "glLinkProgram:";
        print_log(program);
        return 0;
    }

    // ----- BIND TO SHADER VARIABLES -----
    const char* attribute_name = v_coord_name.c_str();
    attribute_v_coord = glGetAttribLocation(program, attribute_name);
    if (-1 == attribute_v_coord)
    {
     cerr << "Could not bind attribute " << attribute_name << endl;
     return 0;
 }

   // attribute_name = v_normal_name.c_str();
   // attribute_texcoord = glGetAttribLocation(program, attribute_name);
   // if (-1 == attribute_texcoord)
   // {
   //     cerr << "Could not bind attribute " << attribute_name << endl;
   //     return 0;
   // }

 const char * uniform_name = mvp_name.c_str();
 uniform_mvp = glGetUniformLocation(program, uniform_name);
 if (-1 == uniform_mvp)
 {
     cerr << "Could not bind uniform " << uniform_name << endl;
     return 0;
 }

 return 1;
}

void onIdle()
{
  // compute mvp

  // move everything back 4 units
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));

	glm::vec3 cameraLocation = glm::vec3(-3.0, 2.0, 4.0);
	glm::vec3 lookTowards = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

	glm::mat4 view = glm::lookAt(cameraLocation, lookTowards, up);

    // translate world to 2D screen
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 10.0f);

    // rotate model 45 degrees every second
    float angle = glutGet(GLUT_ELAPSED_TIME)/200*10; // 45 degrees a second
    glm::vec3 axis_y(1.0, 0.0, 0.0);
    // glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);

    // multiply it all through to get model-view-projection matrix (with an animation at the start)
    // glm::mat4 mvp = projection * view * model * anim;
    glm::mat4 mvp = projection * view * model;

    glUseProgram(program);

    // send this data through to our vertex shader
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    glutPostRedisplay();

}

void onDisplay()
{
    /* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

    // send vertices and normals to shaders
	glEnableVertexAttribArray(attribute_v_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
	glVertexAttribPointer(
    attribute_v_coord,         // attribute
    4,                         // number of elements per vertex, here (x,y,z,w)
    GL_FLOAT,                  // the type of each element
    GL_FALSE,                  // take our values as-is
    0,                         // no extra data between each position
    0                          // offset of first element
    );

    // glEnableVertexAttribArray(attribute_v_normal);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
    // glVertexAttribPointer(
    // attribute_v_normal,         // attribute
    // 3,                         // number of elements per vertex, here (x,y,z)
    // GL_FLOAT,                  // the type of each element
    // GL_FALSE,                  // take our values as-is
    // 0,                         // no extra data between each position
    // 0                          // offset of first element
    // );

    // push each element in buffer_vertices to the vertex shader
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);

    // use glGetBufferParameteriv to grab the buffer size so we don't have to declare cube_elements.
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    /* Push each element in buffer_vertices to the vertex shader */
	// glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribute_v_coord);
	// glDisableVertexAttribArray(attribute_v_normal);

    /* Display the result */
	glutSwapBuffers();

}

void onReshape(int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void free_resources()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_mesh_vertices);
	// glDeleteBuffers(1, &vbo_mesh_normals);
	glDeleteBuffers(1, &ibo_mesh_elements);
}

int main(int argc, char* argv[])
{
  // Glut-related initialising functions 
	glutInit(&argc, argv);
	glutInitContextVersion(2,0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(SCREEN_X, SCREEN_Y);
	glutCreateWindow(TITLE.c_str());

    // Extension wrangler initialising 
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status)
	{
		cout << "Error: " << glewGetErrorString(glew_status) << endl;
		return EXIT_FAILURE;
	}

	if (!GLEW_VERSION_2_0)
	{
		cerr << "Error: your graphic card does not support OpenGL 2.0" << endl;
		return 1;
	}

    // When all init functions run without errors,
    // the program can initialise the resources 
	if (1 == init_resources())
	{
        /* We can display it if everything goes OK */
		glutDisplayFunc(onDisplay);
		glutReshapeFunc(onReshape);
		glutIdleFunc(onIdle);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutMainLoop();
	}

    // If the program exits in the usual way,
    // free resources and exit with a success 
	free_resources();

	return EXIT_SUCCESS;
}