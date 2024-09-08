#include "stdafx.h"
#include "LN/Renderer/VertexArray.h"
#include "RendererAPI.h"
#include "Renderer.h"

namespace LN {

	void Renderer::BeginSence()
	{
	}

	void Renderer::EndSence()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertxeArray)
	{
		vertxeArray->Bind();

	}

}

