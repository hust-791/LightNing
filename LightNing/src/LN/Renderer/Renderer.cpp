#include "stdafx.h"
#include "LN/Renderer/VertexArray.h"
#include "RendererAPI.h"
#include "RendererCommand.h"
#include "Camera.h"
#include "Renderer.h"

namespace LN {

	Renderer::SenceData* Renderer::m_SenceData = new Renderer::SenceData;

	void Renderer::BeginSence(Camera& camera)
	{
		m_SenceData->ViewProjectionMatrix = camera.GetViewProjection();
	}

	void Renderer::EndSence()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertxeArray)
	{
		shader->Bind();
		shader->SetUniformMatrix4fv("u_ViewProjectionMatrix", 1, GL_FALSE, m_SenceData->ViewProjectionMatrix);
		vertxeArray->Bind();
		RendererCommand::DrawIndexed(vertxeArray);
	}

}

