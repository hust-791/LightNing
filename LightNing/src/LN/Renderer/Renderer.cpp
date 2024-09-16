#include "stdafx.h"
#include "LN/Renderer/VertexArray.h"
#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Camera.h"
#include "Renderer.h"

namespace LN {

	Renderer::SenceData* Renderer::m_SenceData = new Renderer::SenceData;

	void Renderer::BeginScene(Camera* camera)
	{
		m_SenceData->ViewProjectionMatrix = camera->GetViewProjection();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertxeArray)
	{
		shader->Bind();
		shader->SetUniform1i("u_Texture", 1);
		shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		shader->SetUniformMatrix4fv("u_Transform", 1, GL_FALSE, glm::mat4(1.0));
		shader->SetUniformMatrix4fv("u_ViewProjection", 1, GL_FALSE, m_SenceData->ViewProjectionMatrix);
		vertxeArray->Bind();
		RenderCommand::DrawIndexed(vertxeArray);
	}

}

