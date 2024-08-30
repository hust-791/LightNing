#include "stdafx.h"
#include "GraphicsContext.h"

#include "LN/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace LN {


    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::en_None:    LN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::en_OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
