#include "stdafx.h"


namespace LN {

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

}

