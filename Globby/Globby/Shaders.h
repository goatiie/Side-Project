typedef unsigned int u32;

#pragma once
class Shaders
{

public:

	int createTexture(char* vPath, char *fPath);
	void clean(char* vPath, char *fPath, int number);

	char cwd[256];

	GLuint m_vTexture;
	GLuint m_fTexture;

	char vertexPath[256];
	char framentPath[256];

	u32 program;
	
	//print the openglError
	int printOpenGLError(void);

	//print the shader info log
	void printShaderInfoLog(GLuint shader);

	//print out program info log
	void printProgramInfoLog(GLuint obj);

	Shaders(void);
	~Shaders(void);
};