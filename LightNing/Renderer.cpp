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

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertxeArray,const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetInt("u_Texture", 1);
		shader->SetFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader->SetMat4("u_Transform", transform);
		shader->SetMat4("u_ViewProjection",m_SenceData->ViewProjectionMatrix);
		vertxeArray->Bind();
		RenderCommand::DrawIndexed(vertxeArray);
	}

}

