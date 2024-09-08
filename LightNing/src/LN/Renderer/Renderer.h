#pragma once
class VertexArray;
class IndexBuffer;
class Shader;

namespace LN
{

	class Renderer
	{
	public:
		static void BeginSence();
		static void EndSence();

		static void Submit(const Ref<VertexArray>& vertxeArray);

		inline static RendererAPI::API GetAPI(){ return RendererAPI::GetAPI(); }

	};

}
