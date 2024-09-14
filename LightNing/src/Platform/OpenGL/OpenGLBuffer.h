#pragma once

namespace LN
{
	class OpenGLVertexBuffer :public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertex, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind()const override;
		void UnBind()const override;

		void SetLayout(const VertexBufferLayout& layout) override;
		const VertexBufferLayout& GetBufferLayout() override;

	private:
		unsigned int m_rendererID;
		VertexBufferLayout m_bufferLayout;
	};


	class OpenGLIndexBuffer :public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* vertex, uint32_t size);
		~OpenGLIndexBuffer();

		void Bind()const override;
		void UnBind()const override;

		int GetCount()const override;

	private:
		unsigned int m_rendererID;
		uint32_t m_count;
	};
}