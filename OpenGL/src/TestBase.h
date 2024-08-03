#pragma once

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
	};


	class TestMenu :public TestBase
	{
	public:
		TestMenu(TestBase*& test);
		virtual ~TestMenu();

		void OnImGuiRender() override;

		template<class T>
		void RegisterTest(const std::string& name)
		{
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		TestBase*& m_currentTest;
		std::vector<std::pair<std::string, std::function< TestBase* ()>>> m_Tests;
	};
}

