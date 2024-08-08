#pragma once
#include "Event.h"

namespace Test
{
	class TestBase
	{
	public:
		TestBase() {}
		virtual ~TestBase() {}

		virtual void OnUpdata(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void OnEvent(Event& e){}

	};


	class TestMenu :public TestBase
	{
	public:
		TestMenu(GLFWwindow* window);
		virtual ~TestMenu();

		void OnImGuiRender() override;

		virtual void OnEvent(Event& e) override
		{
			EventDispatcher dispatcher(e);

			dispatcher.Dispatch<MouseButtonPressedEvent>([this](auto&&... args) -> decltype(auto) { return this->OnMouseButtonPressed(std::forward<decltype(args)>(args)...);});

		}

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e)
		{
			std::cout << "TestMenu pressed!" << std::endl;
			return false;
		}

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
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		static TestBase* m_currentTest;
		GLFWwindow* m_window;
		std::vector<std::pair<std::string, std::function< TestBase* ()>>> m_Tests;
	};
}

