#include <fstream>
#include <ios>

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glext.h"

int LoadShader(const char* fileName, char** shaderSource);

GLuint BuildShaders(GLuint vertexShader, GLuint fragmentShader);
