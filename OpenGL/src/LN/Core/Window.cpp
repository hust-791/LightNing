#include "stdafx.h"
#include "Window.h"

#ifdef LN_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif


namespace LN
{
	Scope<Window> Window::Create(const WindowSetting& set)
	{
	#ifdef LN_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(set);
	#else
			LN_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
	#endif

	}
}