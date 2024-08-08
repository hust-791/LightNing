#include "stdafx.h"
#include "TestBase.h"


using namespace Test;

TestBase* TestMenu::m_currentTest = nullptr;

TestMenu::TestMenu(GLFWwindow* window) :m_window(window)
{
	SetCurrentTest(this);
}

TestMenu::~TestMenu()
{
}

void TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str()))
		{
			m_currentTest = test.second();
		}
	}
}
