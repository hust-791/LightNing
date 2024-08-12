#pragma once
#include "TestBase.h"

namespace Test
{
	struct Camera
	{
		// 相机位置和方向
		glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight, worldUp;
	
		double lastX, lastY;
		bool enable,firstMouse;
		float yaw, pitch, fov;
		float speed, sensitivity;

		Camera() :cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)),
				cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
				worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
				lastX(0), lastY(0), 
				enable(true), firstMouse(true), 
				yaw(-90.0f), pitch(0.0f), fov(45.0f),
				speed(0.1f) , sensitivity(0.03f)
		{
			UpdataCamera();
		}

		void UpdataCamera()
		{
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(front);

			cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
			cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
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
		bool OnKeyPressed(KeyPressedEvent& e);
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
