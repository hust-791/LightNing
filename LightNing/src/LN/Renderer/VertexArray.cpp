#include "stdafx.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "VertexArray.h"


namespace LN 
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::en_None: LN_CORE_ASSERT(false, "VertexArray: RendererAPI invalid !"); return nullptr;
			case RendererAPI::API::en_OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI !")
		return nullptr;
	}
}