#include "stdafx.h"
#include "TextureTest.h"
#include "Vendor/imGUI/imgui_demo.cpp"

namespace Test
{
#define WIDTH 1280.0f
#define HIGTH 720.0f

	TextureTest::TextureTest(GLFWwindow* window) :TestBase(window), m_rotation{ 0.0f,0.0f,0.0f }, m_isAuto{ 0,0,0 }
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
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indicesCube, 36);
		m_IBO_line = std::make_unique<IndexBuffer>(indicesLine, 24);

		m_texture = std::make_unique<Texture>("res/texture/kun.jpeg");

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

		glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation[0]), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation[1]), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		//cube
		m_shader->Bind();
		m_texture->Bind(1);

		m_shader->SetUniform1i("u_Texture", 1);
		m_shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		// 设置模型矩阵
		m_shader->SetUniformMatrix4fv("model", 1, GL_FALSE, modelMatrix);
		// 设置视图矩阵
		glm::mat4 view = m_camera.GetCameraMatrix();
		m_shader->SetUniformMatrix4fv("view", 1, GL_FALSE, view);
		// 设置投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(m_camera.fov), WIDTH / HIGTH, 0.1f, 100.0f);
		m_shader->SetUniformMatrix4fv("projection", 1, GL_FALSE, projection);
		// DrawCall
		renderer.Draw(*m_VAO, *m_IBO, GL_TRIANGLES, *m_shader);


		//Line
		m_texture->Bind();
		m_shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);
		// DrawCall
		renderer.Draw(*m_VAO, *m_IBO_line, GL_LINES, *m_shader);
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
		dispatcher.Dispatch<MouseButtonPressedEvent>(EVENT_BIND_FUNC(TextureTest::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(EVENT_BIND_FUNC(TextureTest::OnMouseReleased));

		dispatcher.Dispatch<MouseMoveEvent>(EVENT_BIND_FUNC(TextureTest::OnMousMove));
		dispatcher.Dispatch<MouseScrolledEvent>(EVENT_BIND_FUNC(TextureTest::OnMousScroll));
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
				glfwSetInputMode(TestMenu::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwGetCursorPos(TestMenu::GetWindow(), &m_camera.lastX, &m_camera.lastY);
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
		if (m_camera.firstMouse)
		{
			m_camera.lastX = e.m_xPos;
			m_camera.lastY = e.m_yPos;
			m_camera.firstMouse = false;
		}

		float xoffset = float(m_camera.lastX - e.m_xPos);
		float yoffset = float(e.m_yPos - m_camera.lastY);
		m_camera.lastX = e.m_xPos;
		m_camera.lastY = e.m_yPos;

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_camera.yaw += xoffset;
		m_camera.pitch += yoffset;

		if (m_camera.pitch > 89.0f)
			m_camera.pitch = 89.0f;
		if (m_camera.pitch < -89.0f)
			m_camera.pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_camera.yaw)) * cos(glm::radians(m_camera.pitch));
		direction.y = sin(glm::radians(m_camera.pitch));
		direction.z = sin(glm::radians(m_camera.yaw)) * cos(glm::radians(m_camera.pitch));
		m_camera.cameraFront = glm::normalize(direction);

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
}