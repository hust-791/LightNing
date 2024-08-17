#include "stdafx.h"
#include "TestBase.h"


namespace LN
{
	TestMenu::TestMenu()
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

	Scope<TestMenu> TestMenu::Create()
	{
		return CreateScope<TestMenu>();
	}
}
