#pragma once

namespace LN
{
	class Event;
	class Window;
	class TestMenu;
	class Layer;
	class Camera;
	class ImGuiLayer;
	class LayerStack;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

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
		ImGuiLayer* m_ImGuiLayer;
		Ref<Camera> m_Camera;

		Scope<TestMenu> m_TestMenu;
		LayerStack m_LayerStack;

		bool m_Running = true;
	};
}

