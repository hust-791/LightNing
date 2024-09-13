#pragma once
class Shader;

namespace LN
{
	class VertexArray;
	class IndexBuffer;
	class Camera;

	class Renderer
	{
	public:
		static void BeginScene(Camera* camera);
		static void EndScene();

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
