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

		virtual void OnMouseClick(GLFWwindow* window, int button, int action, int mods) {
			ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
			std::cout << "base click!" << std::endl; }
		virtual void OnMouseMove(GLFWwindow* window, double xpos, double ypos){}
		virtual void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset){}

	};


	class TestMenu :public TestBase
	{
	public:
		TestMenu(GLFWwindow* window);
		virtual ~TestMenu();

		void OnImGuiRender() override;

		virtual void OnMouseClick(GLFWwindow* window, int button, int action, int mods) {
			ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
			std::cout << "TestMenu click!" << std::endl;
		}

		void SetCurrentTest(TestBase* test)
		{
			m_currentTest = test;
			glfwSetMouseButtonCallback(m_window, MouseClickCallBack);
		}

		static TestBase*& GetCurrentTest()
		{
			return m_currentTest;
		}


		static void MouseClickCallBack(GLFWwindow* window, int button, int action, int mods);

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

