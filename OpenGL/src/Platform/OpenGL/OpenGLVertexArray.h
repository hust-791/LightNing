#pragma once

namespace LN {

	class VertexBuffer;
	class IndexBuffer;

	class OpenGLVertexArray :public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind()const override;
		void UnBind()const override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vb) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& ib) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffer()const override;
		const Ref<IndexBuffer>& GetIndexBuffer()const override;

	private:
		unsigned int m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffers;
	};
}