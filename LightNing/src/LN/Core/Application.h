#pragma once

int main(int argc, char** argv);

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

		void Close();

		Window& GetWindow() { return *m_Window; }

		TestMenu& GetTestMenu() { return *m_TestMenu; }

		static Application& Get() { return *s_Instance; }

	private:
		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		Ref<Camera> m_Camera;

		Scope<TestMenu> m_TestMenu;
		LayerStack m_LayerStack;

		bool m_Running = true;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}

