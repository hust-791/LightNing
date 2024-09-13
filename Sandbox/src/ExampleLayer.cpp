#include "LN.h"
#include "ExampleLayer.h"


ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer"), m_rotation{ 0.0f,0.0f,0.0f }, m_isAuto{ 0,0,0 }
{
	// ������ɫ������
	m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

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
	size_t indicesCube[] = {
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
	size_t indicesLine[] = {
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

	m_VertexArray = LN::VertexArray::Create();

	LN::Ref<LN::VertexBuffer> vertexBuffer = LN::VertexBuffer::Create(vertices, sizeof(vertices));

	LN::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	vertexBuffer->SetLayout(layout);

	m_VertexArray->AddVertexBuffer(vertexBuffer);
	LN::Ref<LN::IndexBuffer> indexBuffer = LN::IndexBuffer::Create(indicesCube, sizeof(indicesCube) / sizeof(size_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_texture = std::make_unique<Texture>("res/texture/kun.jpeg");
	m_texture->Bind();

	m_Camera = LN::CreateRef<LN::EditorCamera>();
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(float ts)
{
	m_Camera->OnUpdate(ts);

	// Render
	LN::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	LN::RenderCommand::Clear();

	LN::Renderer::BeginScene(m_Camera.get());

	LN::Renderer::Submit(m_Shader, m_VertexArray);

	LN::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnEvent(LN::Event& e)
{
}
