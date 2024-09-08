#include "stdafx.h"
#include "RendererAPI.h"
#include <Platform/OpenGL/OpenGLRendererAPI.h>
 

namespace LN
{
	RendererAPI::API RendererAPI::s_API = API::en_OpenGL;


	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::en_None:    LN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::en_OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}