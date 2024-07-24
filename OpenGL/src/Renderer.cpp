#include "glad.h"
#include <iostream>
#include "Renderer.h"


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* func, const char* fileName, const int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") " << func << " " << fileName << " " << line << std::endl;
		return false;
	}
	return true;
}