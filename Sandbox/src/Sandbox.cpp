#include "LN.h"
#include "LN/Core/EntryPoint.h"
#include "ExampleLayer.h"

class Sandbox : public LN::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

LN::Application* LN::CreateApplication()
{
	return new Sandbox();
}
