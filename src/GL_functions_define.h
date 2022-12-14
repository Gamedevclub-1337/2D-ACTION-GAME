#ifndef _GL_FUNCTIONS_DEFINE_H
#define _GL_FUNCTIONS_DEFINE_H
#include <GL/GL.h>
#include "wglext.h"
// NOTE(Mounir): This header better be replaced with a loading library in the future like glew cause this work of getting each opengl function alone is stupid and tedious !!!

// NOTE(Mounir): The function pointers for the opengl functions we'll need here


#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893


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
typedef void (*GL_GETSHADERIV) (GLuint, GLenum, GLint*);
typedef void (*GL_GETSHADERINFOLOG) (GLuint, GLsizei, GLsizei*, char*);
typedef void (*GL_DELETEPROGRAM) (GLuint);
typedef void (*GL_GETPROGRAMIV) (GLuint, GLenum, GLint*);
typedef void (*GL_GETPROGRAMINFOLOG) (GLuint, GLsizei, GLsizei*, char*);


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
GL_GETSHADERIV	glGetShaderiv;
GL_GETSHADERINFOLOG	glGetShaderInfoLog;
GL_DELETEPROGRAM	glDeleteProgram;
GL_GETPROGRAMIV	glGetProgramiv;
GL_GETPROGRAMINFOLOG	glGetProgramInfoLog;

void gl_GetFunctions();
#endif //_GL_FUNCTIONS_DEFINE_H
