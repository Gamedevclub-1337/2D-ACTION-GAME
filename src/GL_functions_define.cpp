#include "GL_functions_define.h"

void gl_GetFunctions()
{
	glCreateShader = (GL_CREATESHADER)wglGetProcAddress("glCreateShader");
	glBindBuffer = (GL_BINDBUFFER)wglGetProcAddress("glBindBuffer");
    glUseProgram = (GL_USEPROGRAM)wglGetProcAddress("glUseProgram");
    glBufferData = (GL_BUFFERDATA)wglGetProcAddress("glBufferData");
	glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
	glShaderSource  = (GL_SHADERSOURCE)wglGetProcAddress("glShaderSource");
	glCompileShader = (GL_COMPILESHADER)wglGetProcAddress("glCompileShader");
	glCreateProgram = (GL_CREATEPROGRAM)wglGetProcAddress("glCreateProgram");
	glAttachShader  = (GL_ATTACHSHADER)wglGetProcAddress("glAttachShader");
    glLinkProgram   = (GL_LINKPROGRAM)wglGetProcAddress("glLinkProgram");
	glDeleteShader  = (GL_DELETESHADER)wglGetProcAddress("glDeleteShader");
    glVertexAttribPointer = (GL_VERTEXATTRIBPOINTER)wglGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (GL_ENABLEVERTEXATTRIBARRAY)wglGetProcAddress("glEnableVertexAttribArray");
	glGenVertexArrays = (GL_GENVERTEXARRAYS)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (GL_BINDVERTEXARRAY)wglGetProcAddress("glBindVertexArray");
    glGetUniformLocation = (GL_GETUNIFORMLOCATION)wglGetProcAddress("glGetUniformLocation");
	glUniform1f = (GL_UNIFORM1F)wglGetProcAddress("glUniform1f");
	glGetShaderInfoLog = (GL_GETSHADERINFOLOG)wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderiv = (GL_GETSHADERIV)wglGetProcAddress("glGetShaderiv");
	glDeleteProgram = (GL_DELETEPROGRAM)wglGetProcAddress("glDeleteProgram");
	glGetProgramiv = (GL_GETPROGRAMIV)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (GL_GETPROGRAMINFOLOG)wglGetProcAddress("glGetProgramInfoLog");
}

