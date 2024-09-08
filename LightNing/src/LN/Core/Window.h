#pragma once

namespace LN
{
	struct WindowSetting
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowSetting(const std::string& title = "LN Engine", uint32_t width = 800, uint32_t height = 450) : Title(title), Width(width), Height(height) {}
	};

	class Event;

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() = default;

		virtual void OnUpdata() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Scope<Window> Create(const WindowSetting& set = WindowSetting());
	};

}