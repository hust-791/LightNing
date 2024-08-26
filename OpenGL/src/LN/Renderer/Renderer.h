#pragma once
class VertexArray;
class IndexBuffer;
class Shader;

namespace LN {

	class Renderer
	{
	public:
		void Draw(VertexArray& va, IndexBuffer& ib, unsigned int drawModel, Shader& shader);
		void Clear();
	};

}

