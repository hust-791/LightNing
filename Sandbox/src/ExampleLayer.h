#pragma once


class ExampleLayer : public LN::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(float ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(LN::Event& e) override;

private:
	LN::Ref<Shader> m_Shader;
	LN::Ref<LN::VertexArray> m_VertexArray;

	std::unique_ptr<Texture> m_texture;

	//Ä£ÐÍ¾ØÕó
	float m_rotation[3];
	bool  m_isAuto[3];

	LN::Ref<LN::Camera> m_Camera;
};

