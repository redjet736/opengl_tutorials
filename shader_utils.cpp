#include "shader_utils.h"

using namespace std;

/**
 * Display compilation errors from the OpenGL shader compiler
 */
 void print_log(GLuint object)
 {
  GLint log_length = 0;
  if (glIsShader(object))
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else if (glIsProgram(object))
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else {
    cerr << "printlog: Not a shader or a program\n";
    return;
  }

  char* log = (char*)malloc(log_length);

  if (glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
    glGetProgramInfoLog(object, log_length, NULL, log);

  cerr << log;
  free(log);
}

//  /*
// * Store all the file's contents in memory, useful to pass shaders
// * source code to OpenGL
// */
// Problem:
// *  We should close the input file before the return NULL; statements but this would lead to a lot of repetition (DRY)
// *   -you could solve this by using goto or by abusing switch/for/while + break or by building an if else mess
// *  better solution: let the user handle the File: char* file_read(const FILE* input)

string file_read(const string filename)
{
  // open file
  ifstream filestream;
  filestream.open(filename.c_str());

  if (!filestream.is_open())
  {
    return NULL;
  }

  // read contents of file into string
  string contents(
    (istreambuf_iterator<char>(filestream)),
    (istreambuf_iterator<char>())
    );

  filestream.close();

  return contents;
}

// Compile the shader from file 'filename', with error handling.
// returns 0 on failure, non 0 on success
GLuint create_shader(string filename, GLenum type)
{
  string contents = file_read(filename);
  const GLchar* source = contents.c_str();
  if (source == NULL) 
  {
    cerr << "Error opening " << filename << endl;
    return 0;
  }
  GLuint res = glCreateShader(type);
  const GLchar* sources[2] = 
  {
#ifdef GL_ES_VERSION_2_0
  "#version 100\n"
  "#define GLES2\n",
#else
  "#version 120\n",
#endif
  source
  };
  glShaderSource(res, 2, sources, NULL);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (GL_FALSE == compile_ok) 
  {
    cerr << filename << endl;
    print_log(res);
    glDeleteShader(res);
    return 0;
  }

  return res;
}