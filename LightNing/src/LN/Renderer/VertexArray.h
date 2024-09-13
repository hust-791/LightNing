#pragma once


namespace LN {

	class IndexBuffer;
	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& ib) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer()const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer()const = 0;

		static Ref<VertexArray> Create();
	};

}

