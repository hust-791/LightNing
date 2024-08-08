#include "stdafx.h"
#include "TextureTest.h"
#include "Vendor/imGUI/imgui_demo.cpp"

namespace Test
{
	#define WIDTH 1280.0f
	#define HIGTH 720.0f

	// 相机位置和方向
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float fov = 45.0f;


	TextureTest::TextureTest() :m_rotation{ 0.0f,0.0f,0.0f }, m_isAuto{ 0,0,0 }
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
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		m_shader->SetUniformMatrix4fv("view", 1, GL_FALSE, view);
		// 设置投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(fov), WIDTH / HIGTH, 0.1f, 100.0f);
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
		ImGui::Checkbox("Auto Z", m_isAuto + 2*sizeof(bool));
	}

	void TextureTest::OnMouseClick(GLFWwindow* window, int button, int action, int mods)
	{
		std::cout << "TextureTest click!" << std::endl;
	}
	void TextureTest::OnMouseMove(GLFWwindow* window, double xpos, double ypos)
	{
	}
	void TextureTest::OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
	}
}
