#ifndef _GL_FUNCTIONS_DEFINE_H
#define _GL_FUNCTIONS_DEFINE_H
#include <GL/GL.h>
#include "wglext.h"
// NOTE(Mounir): This header better be replaced with a loading library in the future like glew cause this work of getting each opengl function alone is stupid and tedious !!!

// NOTE(Mounir): The function pointers for the opengl functions we'll need here
typedef void   (*GL_GENBUFFERS) (GLsizei, GLuint*);
typedef GLuint (*GL_GETUNIFORMLOCATION) (GLuint, const char*);
typedef void   (*GL_DRAWELEMENT) (GLenum, GLsizei, GLenum, const void*);
typedef void   (*GL_SHADERSOURCE) (GLuint, GLsizei, const char* const*, const int*);
typedef void   (*GL_BUFFERDATA) (GLenum, ptrdiff_t, const void*, GLenum);
typedef void   (*GL_UNIFORM1F) (GLint, GLfloat);
typedef void   (*GL_BINDVERTEXARRAY) (GLuint);
typedef void   (*GL_BINDBUFFER) (GLenum, GLuint);
typedef void   (*GL_LINKPROGRAM) (GLuint);
typedef void   (*GL_BINDVERTEXARRAY) (GLuint);
typedef void   (*GL_GENVERTEXARRAYS) (GLsizei, GLuint*);
typedef void   (*GL_VERTEXATTRIBPOINTER) (GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef void   (*GL_ENABLEVERTEXATTRIBARRAY) (GLuint);
typedef void   (*GL_COMPILESHADER) (GLuint);
typedef void   (*GL_DELETESHADER) (GLuint);
typedef void   (*GL_USEPROGRAM) (GLuint);
typedef GLuint (*GL_CREATESHADER) (GLenum);
typedef GLuint (*GL_CREATEPROGRAM) ();
typedef void   (*GL_ATTACHSHADER) (GLuint, GLuint);
typedef BOOL WINAPI WGLCHOOSEPIXELFORMATARB (HDC, const int *, const FLOAT *, UINT, int * , UINT *);
typedef HGLRC (*WGLCREATECONTEXTATTRIBSARB) (HDC, HGLRC, const int*);

GL_CREATESHADER     glCreateShader;
GL_BINDBUFFER		glBindBuffer;
GL_USEPROGRAM		glUseProgram;
GL_BUFFERDATA		glBufferData;
GL_GENBUFFERS       glGenBuffers;
GL_SHADERSOURCE		glShaderSource;
GL_COMPILESHADER	glCompileShader;
GL_CREATEPROGRAM	glCreateProgram;
GL_ATTACHSHADER		glAttachShader;
GL_LINKPROGRAM		glLinkProgram;
GL_DELETESHADER		glDeleteShader;
GL_VERTEXATTRIBPOINTER glVertexAttribPointer;
GL_ENABLEVERTEXATTRIBARRAY glEnableVertexAttribArray;
GL_GENVERTEXARRAYS	glGenVertexArrays;
GL_BINDVERTEXARRAY	glBindVertexArray;
GL_GETUNIFORMLOCATION glGetUniformLocation;
GL_UNIFORM1F		glUniform1f;

void gl_GetFunctions();
#endif //_GL_FUNCTIONS_DEFINE_H