#pragma once


namespace LN
{
	class GraphicsContext;

	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowSetting& set);
		virtual ~WindowsWindow();

		void OnUpdata() override;

		unsigned int GetWidth() const override { return m_data.Width; }
		unsigned int GetHeight() const override { return m_data.Height; }

		void SetVSync(bool enable) override;
		bool IsVSync() const override;

		void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }

		void* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowSetting& set);
		void Shutdowm();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};

}