#include "stdafx.h"
#include "LN/Renderer/VertexArray.h"
#include "LN/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "RendererCommand.h"


namespace LN {
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;


	void RendererCommand::SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void RendererCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RendererCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		s_RendererAPI->DrawIndexed(vertexArray);
	}

}