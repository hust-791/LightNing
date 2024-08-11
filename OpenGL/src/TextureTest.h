#pragma once
#include "TestBase.h"

namespace Test
{
	struct Camera
	{
		// 相机位置和方向
		glm::vec3 cameraPos, cameraFront, cameraUp;

		double lastX, lastY;
		bool firstMouse;
		float yaw, pitch, fov;

		Camera()
		{
			cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

			lastX = lastY = 0;
			firstMouse = true;
			pitch = 0.f;
			yaw = -90.f;
			fov = 45.0f;
		}

		glm::mat4 GetCameraMatrix() const
		{
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		};
	};

	class TextureTest : public TestBase
	{
	public:
		TextureTest(GLFWwindow* window);
		virtual ~TextureTest();

		void OnUpdata(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

		virtual void OnEvent(Event& e)override;
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMousMove(MouseMoveEvent& e);
		bool OnMousScroll(MouseScrolledEvent& e);
	private:
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<IndexBuffer> m_IBO_line;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<Texture> m_texture;

		//模型矩阵
		float m_rotation[3];
		bool  m_isAuto[3];

		Camera m_camera;
	};
}
