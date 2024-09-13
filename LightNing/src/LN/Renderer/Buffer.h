#pragma once

namespace LN
{
	enum class ShaderDataType
	{
		None = 0, Float, UInt, Byte, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::UInt:     return 4;
		case ShaderDataType::Byte:     return 1;
		case ShaderDataType::Bool:     return 1;
		}

		LN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	struct VertexBufferElement
	{
		ShaderDataType type;
		unsigned int count;
		bool normalized;
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
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ ShaderDataType::Float,count,false });
			m_Stride += count * ShaderDataTypeSize(ShaderDataType::Float);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ ShaderDataType::UInt,count,true });
			m_Stride += count * ShaderDataTypeSize(ShaderDataType::UInt);
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ ShaderDataType::Byte,count,true });
			m_Stride += count * ShaderDataTypeSize(ShaderDataType::Byte);
		}
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