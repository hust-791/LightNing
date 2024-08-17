#pragma once
#include "LN/Event/Event.h"

namespace LN
{
	class TestBase
	{
	public:
		TestBase(){}
		virtual ~TestBase() {}

		virtual void OnUpdata(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void OnEvent(Event& e){}

	};


	class TestMenu :public TestBase
	{
	public:
		virtual ~TestMenu();

		static Scope<TestMenu> Create();

		void OnImGuiRender() override;

		void SetCurrentTest(TestBase* test) { m_currentTest = test; }

		TestBase*& GetCurrentTest() { return m_currentTest; }


		template<class T>
		void RegisterTest(const std::string& name)
		{
			m_Tests.push_back(std::make_pair(name, []() {return new T(GetWindow()); }));
		}
	private:
		TestMenu();

	private:
		TestBase* m_currentTest;
		std::vector<std::pair<std::string, std::function< TestBase* ()>>> m_Tests;
	};
}

