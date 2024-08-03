#include "stdafx.h"
#include "TestBase.h"


using namespace Test;
TestMenu::TestMenu(TestBase*& test):m_currentTest(test)
{
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
