#include "stdafx.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Buffer.h"


namespace LN
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertex, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::en_None: LN_CORE_ASSERT(false, "VertexBuffer: RendererAPI invalid !"); return nullptr;
			case RendererAPI::API::en_OpenGL: return CreateRef<OpenGLVertexBuffer>(vertex, size);
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI !")
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* index, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::en_None: LN_CORE_ASSERT(false, "IndexBuffer: RendererAPI invalid !"); return nullptr;
		case RendererAPI::API::en_OpenGL: return CreateRef<OpenGLIndexBuffer>(index, size);
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI !")
		return nullptr;
	}

}