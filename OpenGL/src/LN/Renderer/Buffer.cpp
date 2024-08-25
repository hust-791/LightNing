#include "stdafx.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Buffer.h"

namespace LN
{
	VertexBuffer* VertexBuffer::Create(float* vertex, size_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::en_None: LN_CORE_ASSERT(false, "VertexBuffer: RendererAPI invalid !"); return nullptr;
			case RendererAPI::API::en_OpenGL: return new OpenGLVertexBuffer(vertex, size);
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI !")
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(size_t* index, size_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::en_None: LN_CORE_ASSERT(false, "IndexBuffer: RendererAPI invalid !"); return nullptr;
		case RendererAPI::API::en_OpenGL: return new OpenGLIndexBuffer(index, size);
		}

		LN_CORE_ASSERT(false, "Unknown RendererAPI !")
		return nullptr;
	}


	unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		//ASSERT(false);
		return 0;
	}

	template<>
	void VertexBufferLayout::Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void VertexBufferLayout::Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void VertexBufferLayout::Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

}