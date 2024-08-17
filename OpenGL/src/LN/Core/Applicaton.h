#pragma once

namespace LN
{
	class Window;
	class TestMenu;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void Run();

		void Close();

		Window& GetWindow() { return *m_Window; }

		TestMenu& GetTestMenu() { return *m_TestMenu; }

		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;
		Scope<TestMenu> m_TestMenu;

		bool m_Running = true;
	};
}

