#pragma once
class VertexArray;
class IndexBuffer;
class Shader;

namespace LN
{

	class Renderer
	{
	public:
		static void BeginSence(Camera& camera);
		static void EndSence();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertxeArray);

		inline static RendererAPI::API GetAPI(){ return RendererAPI::GetAPI(); }

	private:
		struct SenceData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		
		static SenceData* m_SenceData;
	};

}
