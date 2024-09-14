#include "stdafx.h"
#include "LN/Renderer/Buffer.h"
#include "OpenGLBuffer.h"

namespace LN
{
	//----------------------------------------------------------
	//---------------------Vertex Buffer------------------------
	//----------------------------------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertex, uint32_t size)
	{
		GLCall(glGenBuffers(1, &m_rendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertex, GL_STATIC_DRAW));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_rendererID));
	}

	void OpenGLVertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGLVertexBuffer::SetLayout(const VertexBufferLayout& layout)
	{
		m_bufferLayout = layout;
	}

	const VertexBufferLayout& OpenGLVertexBuffer::GetBufferLayout()
	{
		return m_bufferLayout;
	}


	//----------------------------------------------------------
	//----------------------Index Buffer------------------------
	//----------------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* vertex, uint32_t size) :m_count(size)
	{
		GLCall(glGenBuffers(1, &m_rendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), vertex, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_rendererID));
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	int OpenGLIndexBuffer::GetCount() const
	{
		return m_count;
	}

}