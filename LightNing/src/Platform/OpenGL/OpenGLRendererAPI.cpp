#include "stdafx.h"
#include "LN/Renderer/Buffer.h"
#include "LN/Renderer/VertexArray.h"
#include "LN/Renderer/RendererAPI.h"
#include "OpenGLRendererAPI.h"

namespace LN {

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL));
	}

}
