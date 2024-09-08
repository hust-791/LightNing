#include "stdafx.h"
#include "LN/Renderer/Buffer.h"
#include "OpenGLBuffer.h"

namespace LN
{
	//----------------------------------------------------------
	//---------------------Vertex Buffer------------------------
	//----------------------------------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertex, size_t size)
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
	//---------------------Vertex Buffer------------------------
	//----------------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(size_t* vertex, size_t size) :m_count((int)size)
	{
		GLCall(glGenBuffers(1, &m_rendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertex, GL_STATIC_DRAW));
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