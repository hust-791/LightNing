#pragma once

namespace LN
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer();

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};



	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer();

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static IndexBuffer* Create(uint32_t* vertices, uint32_t size);
	};

}