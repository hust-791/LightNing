#pragma once
#include "Event.h"

namespace Test
{
	class TestBase
	{
	public:
		TestBase(GLFWwindow* window):m_window(window){}
		virtual ~TestBase() {}

		virtual void OnUpdata(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void OnEvent(Event& e){}

		GLFWwindow* m_window;
	};


	class TestMenu :public TestBase
	{
	public:
		TestMenu(GLFWwindow* window);
		virtual ~TestMenu();

		void OnImGuiRender() override;

		static GLFWwindow* GetWindow() { return m_currentTest->m_window; }

		void SetCurrentTest(TestBase* test)
		{
			m_currentTest = test;
		}

		static TestBase*& GetCurrentTest()
		{
			return m_currentTest;
		}

		template<class T>
		void RegisterTest(const std::string& name)
		{
			m_Tests.push_back(std::make_pair(name, []() {return new T(GetWindow()); }));
		}
	private:
		static TestBase* m_currentTest;

		std::vector<std::pair<std::string, std::function< TestBase* ()>>> m_Tests;
	};
}

