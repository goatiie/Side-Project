#include "Helper.h"

#define SHADER_COUNT 1

Shaders::Shaders(void)
{
}

void Shaders::clean(char *vertexPath, char *fragmentPath, int number)
{
	//do a check to make sure which program to delete
	if(number == 1)
	{
		if(m_vTexture)
			glDeleteShader(m_vTexture);
		if(m_fTexture)
			glDeleteShader(m_fTexture);
		if(program)
			glDeleteProgram(program);
	}
}

int Shaders::printOpenGLError(void)
{
	GLenum glErr;
	int retCode = 0;
	char *err;

	//returns 1 if an OpenGL error occured, 0 otherwise

	glErr = glGetError();
	while(glErr != GL_NO_ERROR)
	{
		err = (char*)gluErrorString(glErr);
		printf("Error:\n%s\n\n", err);
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void Shaders::printShaderInfoLog(GLuint shader)
{
	int infologLen = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	printOpenGLError();

	if(infologLen > 0)
	{
		infoLog = (GLchar*)malloc(infologLen);
		if(infoLog == NULL)
		{
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}

		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		printf("InfoLog:\n%s\n\n", infoLog);
		free(infoLog);
	}
	//check for opengl errors
	printOpenGLError();
}

void Shaders::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if(infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);

		printf("%s\n", infoLog);

		free(infoLog);
	}
}

Shaders::~Shaders(void)
{
}
