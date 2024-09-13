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


};

