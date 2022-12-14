#include "glTools.h"
#include "GL_functions_define.h"

GLuint	compileShader(const char* filepath, GLenum shaderType)
{
	GLuint	shaderID = glCreateShader(shaderType);
	if (shaderID == 0)
	{
		ERROR_LOG("glCreateShader(%d) Failed in : %s, %d\n", filepath, shaderType, __FILE__, __LINE__);
		return (0);
	}
	
	Buffer	shaderBuffer = readFileNullTerminated(filepath);
	if (shaderBuffer.size == 0)
	{
		glDeleteShader(shaderID);
		free(shaderBuffer.mem);
		return (0);
	}
	glShaderSource(shaderID, 1, &((char*)(shaderBuffer.mem)), 0);
	glCompileShader(shaderID);
	
	GLint	shaderCompiled;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
		GLsizei	length = 0;
		char message[2048];
		glGetShaderInfoLog(shaderID, 2048, &length, message);
		ERROR_LOG("compilation %s Failed:\n%s\n", filepath, message);
		glDeleteShader(shaderID);
		shaderID = 0;
	}
	free(shaderBuffer.mem);
	return (shaderID);
}

GLuint	compileProgram(const char* vertexFilepath, const char* fragmentFilepath)
{
	GLuint	shaderProgram = glCreateProgram();
	if (shaderProgram == 0)
	{
		ERROR_LOG("glCreateProgram(%s, %s) Failed in : %s, %d\n", vertexFilepath, fragmentFilepath, __FILE__, __LINE__);
		return (0);
	}
	
	GLuint	vertexShaderID = compileShader(vertexFilepath, GL_VERTEX_SHADER);
	if (vertexShaderID == 0)
	{
		glDeleteProgram(shaderProgram);
		return (0);
	}
	GLuint	fragmentShaderID = compileShader(fragmentFilepath, GL_FRAGMENT_SHADER);
	if (fragmentShaderID == 0)
	{
		glDeleteProgram(shaderProgram);
		return (0);
	}
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);
	glLinkProgram(shaderProgram);
	GLint	programLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programLinked);
	if (programLinked != GL_TRUE)
	{
		GLsizei	length = 0;
		char message[2048];
		glGetProgramInfoLog(shaderProgram, 2048, &length, message);
		ERROR_LOG("Linking %s %s Failed:\n%s\n", vertexFilepath, fragmentFilepath, message);
		glDeleteProgram(shaderProgram);
		shaderProgram= 0;
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	INFO_LOG("Program successfully created : %s, %s\n", vertexFilepath, fragmentFilepath);
	return (shaderProgram);
}
