#include "LN.h"
#include "ExampleLayer.h"


ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer"), m_rotation{ 0.0f,0.0f,0.0f }, m_isAuto{ 0,0,0 }
{
	// 创建着色器程序
	m_Shader = LN::Shader::Create("../LightNing/res/shaders/Basic.shader");

	// Texture
	m_texture = std::make_unique<Texture>("../LightNing/res/texture/kun.jpeg");

	float vertices[] = {
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};
	uint32_t indicesCube[] = {
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
	uint32_t indicesLine[] = {
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

	// Cube
	m_VA_Cube = LN::VertexArray::Create();

	LN::Ref<LN::VertexBuffer> VBO_cube = LN::VertexBuffer::Create(vertices, sizeof(vertices));

	LN::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	VBO_cube->SetLayout(layout);

	m_VA_Cube->AddVertexBuffer(VBO_cube);
	LN::Ref<LN::IndexBuffer> IBO_cube = LN::IndexBuffer::Create(indicesCube, sizeof(indicesCube) / sizeof(uint32_t));
	m_VA_Cube->SetIndexBuffer(IBO_cube);

	// Line
	m_VA_Line = LN::VertexArray::Create();

	LN::Ref<LN::VertexBuffer> VBO_line = LN::VertexBuffer::Create(vertices, sizeof(vertices));
	VBO_line->SetLayout(layout);

	m_VA_Line->AddVertexBuffer(VBO_line);
	LN::Ref<LN::IndexBuffer> IBO_line = LN::IndexBuffer::Create(indicesLine, sizeof(indicesLine) / sizeof(uint32_t));
	m_VA_Line->SetIndexBuffer(IBO_line);

	// Camera
	m_Camera = LN::CreateRef<LN::EditorCamera>(30.0f, 1.778f, 0.1f, 1000.0f);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(LN::Timestep ts)
{
	m_Camera->OnUpdate(ts);

	// Render
	LN::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	LN::RenderCommand::Clear();

	LN::Renderer::BeginScene(m_Camera.get());
	m_texture->Bind(1);
	LN::Renderer::Submit(m_Shader, m_VA_Cube);
	//m_texture->Bind(0);
	//LN::Renderer::Submit(m_Shader, m_VA_Line);

	LN::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnEvent(LN::Event& e)
{
	m_Camera->OnEvent(e);
}
