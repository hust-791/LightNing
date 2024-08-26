#pragma once


namespace LN {

	class VertexArray;

	class RendererAPI
	{
	public:
		enum class API
		{
			en_None = 0, en_OpenGL = 1
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}