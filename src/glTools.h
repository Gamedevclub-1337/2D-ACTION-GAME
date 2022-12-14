#ifndef GL_TOOLS_H
#define GL_TOOLS_H

#include <GL/GL.h>

GLuint	compileShader(const char* filepath, GLenum shadeType);
GLuint	compileProgram(const char* vertexFilepath, const char* fragmentFilpath);

#endif //GL_TOOLS_H
