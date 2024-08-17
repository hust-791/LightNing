#include "stdafx.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"


namespace LN
{

	Scope<Window> Window::Create(const WindowSetting& set)
	{
		return CreateScope<WindowsWindow>(set);
	}

}