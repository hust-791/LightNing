#include "stdafx.h"
#include "OpenGLVertexArray.h"


namespace LN {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::UInt:     return GL_UNSIGNED_INT;
		case ShaderDataType::Byte:     return GL_UNSIGNED_BYTE;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		LN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		LN_CORE_ASSERT(vb->GetBufferLayout().getElement().size(), "Vertex Buffer has no Layout !");

		GLCall(glBindVertexArray(m_RendererID));
		vb->Bind();

		auto& layout = vb->GetBufferLayout();
		const auto& elements = layout.getElement();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];

			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i,
				element.count, 
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE, 
				layout.getStride(), 
				reinterpret_cast<const void*>(offset)));

			offset += element.count * ShaderDataTypeSize(element.type);
		}

		m_VertexBuffers.push_back(vb);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		m_IndexBuffers = ib;
	}

	const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffer()const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer()const
	{
		return m_IndexBuffers;
	}

	void OpenGLVertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void OpenGLVertexArray::UnBind() const
	{
		GLCall(glBindVertexArray(0));
	}
}

