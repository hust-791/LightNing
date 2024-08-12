#include "stdafx.h"

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

void Renderer::Draw(VertexArray& va, IndexBuffer& ib, unsigned int drawModel, Shader& shader)
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(drawModel, ib.GetCount(), GL_UNSIGNED_INT, NULL));
}

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
