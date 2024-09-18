#include "stdafx.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "shader.h"

namespace LN
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::en_None: LN_CORE_ASSERT(false, "Shader: RendererAPI invalid !"); return nullptr;
			case RendererAPI::API::en_OpenGL: return CreateRef<OpenGLShader>(filepath);
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI !")
			return nullptr;
	}
}
