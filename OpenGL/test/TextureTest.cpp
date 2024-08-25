#include "stdafx.h"
#include "LN/Renderer/VertexBufferLayout.h"
#include "LN/Renderer/Buffer.h"
#include "LN/Renderer/VertexArray.h"
#include "LN/Core/Applicaton.h"
#include "TextureTest.h"


namespace LN
{
#define WIDTH 1280.0f
#define HIGTH 720.0f

	TextureTest::TextureTest(): m_rotation{ 0.0f,0.0f,0.0f }, m_isAuto{ 0,0,0 }
	{
		// 创建着色器程序
		m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");

		GLfloat vertices[] = {
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
		};
		unsigned int indicesCube[] = {
			0,1,2,
			2,3,0,
			1,5,6,
			6,2,1,
			4,0,3,
			3,7,4,
			3,2,6,
			6,7,3,
			4,5,1,
			1,0,4,
			5,4,7,
			7,6,5
		};
		unsigned int indicesLine[] = {
			0,1,
			1,2,
			2,3,
			3,0,
			0,4,
			1,5,
			2,6,
			3,7,
			4,5,
			5,6,
			6,7,
			7,4
		};

		// 配置顶点缓冲对象和顶点数组对象
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddVertexBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indicesCube, 36);
		m_IBO_line = std::make_unique<IndexBuffer>(indicesLine, 24);

		m_texture = std::make_unique<Texture>("res/texture/kun.jpeg");

		glEnable(GL_DEPTH_TEST);

		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetCursorPos(window, WIDTH / 2.0, HIGTH / 2.0);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	TextureTest::~TextureTest()
	{
	}

	void TextureTest::OnUpdata(float deltaTime)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (m_isAuto[i])
			{
				if (m_rotation[i] > 360)
					m_rotation[i] = 0;
				m_rotation[i]++;
			}
		}
	}

	void TextureTest::OnRender()
	{
		Renderer renderer;

		glm::mat4 projection = glm::perspective(glm::radians(m_camera.fov), WIDTH / HIGTH, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetCameraMatrix();

		for (int i = 0; i < 10; i++)
		{
			float rad = (float)36 * (i);
			float x = sin(glm::radians(rad)) * 4.0f;
			float z = cos(glm::radians(rad)) * 4.0f + 4.0f;

			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z)) *
									glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation[0] + rad), glm::vec3(0.0f, 1.0f, 0.0f)) *
									glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation[1]), glm::vec3(1.0f, 0.0f, 0.0f)) *
									glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 MatrixProjViewModel = projection * view * modelMatrix;
			//cube
			m_shader->Bind();
			m_texture->Bind(1);

			m_shader->SetUniform1i("u_Texture", 1);
			m_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
			m_shader->SetUniformMatrix4fv("matrixProjViewModel", 1, GL_FALSE, MatrixProjViewModel);
			// DrawCall
			renderer.Draw(*m_VAO, *m_IBO, GL_TRIANGLES, *m_shader);


			//Line
			m_texture->Bind();
			m_shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);
			// DrawCall
			renderer.Draw(*m_VAO, *m_IBO_line, GL_LINES, *m_shader);
		}
	}

	void TextureTest::OnImGuiRender()
	{
		ImGui::SliderFloat3("Rotation", m_rotation, 0.0f, 360.0f);
		ImGui::Checkbox("Auto X", m_isAuto);
		ImGui::Checkbox("Auto Y", m_isAuto + sizeof(bool));
		ImGui::Checkbox("Auto Z", m_isAuto + 2 * sizeof(bool));
	}

	void TextureTest::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(LN_EVENT_BIND_FUNC(TextureTest::OnMousePressed));
		//dispatcher.Dispatch<MouseButtonReleasedEvent>(LN_EVENT_BIND_FUNC(TextureTest::OnMouseReleased));

		dispatcher.Dispatch<MouseMoveEvent>(LN_EVENT_BIND_FUNC(TextureTest::OnMousMove));
		dispatcher.Dispatch<MouseScrolledEvent>(LN_EVENT_BIND_FUNC(TextureTest::OnMousScroll));
		dispatcher.Dispatch<KeyPressedEvent>(LN_EVENT_BIND_FUNC(TextureTest::OnKeyPressed));
	}

	bool TextureTest::OnMousePressed(MouseButtonPressedEvent& e)
	{
		switch (e.GetMouseButton())
		{
			case ButtonLeft:
			{
			}break;

			case ButtonRight:
			{
			}break;

			default:
				break;
		}

		return false;
	}

	bool TextureTest::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		return false;
	}

	bool TextureTest::OnMousMove(MouseMoveEvent& e)
	{
		if (!m_camera.enable)
			return false;

		if (m_camera.firstMouse)
		{
			m_camera.lastX = e.m_xPos;
			m_camera.lastY = e.m_yPos;
			m_camera.firstMouse = false;
		}

		float xoffset = float(e.m_xPos - m_camera.lastX);
		float yoffset = float(m_camera.lastY - e.m_yPos);
		m_camera.lastX = e.m_xPos;
		m_camera.lastY = e.m_yPos;

		m_camera.yaw += xoffset * m_camera.sensitivity;
		m_camera.pitch += yoffset * m_camera.sensitivity;

		if (m_camera.pitch > 89.0f)
			m_camera.pitch = 89.0f;
		if (m_camera.pitch < -89.0f)
			m_camera.pitch = -89.0f;

		m_camera.UpdataCamera();
		return false;
	}

	bool TextureTest::OnMousScroll(MouseScrolledEvent& e)
	{
		if (m_camera.fov >= 1.0f && m_camera.fov <= 45.0f)
			m_camera.fov -= (float)e.m_yOffect;
		else if (m_camera.fov < 1.0f)
			m_camera.fov = 1.0f;
		else if (m_camera.fov > 45.0f)
			m_camera.fov = 45.0f;

		return false;
	}

	bool TextureTest::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
			case EnKeyCode::W:
			{
				m_camera.cameraPos += m_camera.cameraFront * m_camera.speed;
			}break;

			case EnKeyCode::A:
			{
				m_camera.cameraPos -= m_camera.cameraRight * m_camera.speed;
			}break;

			case EnKeyCode::S:
			{
				m_camera.cameraPos -= m_camera.cameraFront * m_camera.speed;
			}break;

			case EnKeyCode::D:
			{
				m_camera.cameraPos += m_camera.cameraRight * m_camera.speed;
			}break;

			case EnKeyCode::Q:
			{
				m_camera.cameraPos += m_camera.worldUp * m_camera.speed;
			}break;

			case EnKeyCode::E:
			{
				m_camera.cameraPos -= m_camera.worldUp * m_camera.speed;
			}break;

			case EnKeyCode::V:
			{
				static bool b = true;
				auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				if (b)
				{
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					glfwGetCursorPos(window, &m_camera.lastX, &m_camera.lastY);
					m_camera.enable = false;
					m_camera.firstMouse = true;
				}
				else
				{
					glfwSetCursorPos(window, WIDTH / 2.0, HIGTH / 2.0);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					m_camera.enable = true;
				}
				b = !b;
			}break;

			case EnKeyCode::LeftShift:
			{
				m_camera.speed += 0.05f;
			}break;

			default: break;
		}

		return false;
	}
}