#pragma once

namespace LN
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type);
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;

	public:
		VertexBufferLayout() :m_Stride(0) {}

		inline const std::vector<VertexBufferElement>& getElement() const { return m_Elements; };
		inline const unsigned int getStride() const { return m_Stride; }

		template<typename T>
		void Push(unsigned int count)
		{
			//static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count);

		template<>
		void Push<unsigned int>(unsigned int count);

		template<>
		void Push<unsigned char>(unsigned int count);
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer(){}

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual const VertexBufferLayout& GetBufferLayout() = 0;

		static Ref<VertexBuffer> Create(float* vertex, size_t size);
	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual int GetCount()const = 0;

		static Ref<IndexBuffer> Create(size_t* index, size_t size);
	};

}