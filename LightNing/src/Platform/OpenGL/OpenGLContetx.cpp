#include "stdafx.h"
#include "LN/Renderer/GraphicsContext.h"
#include "OpenGLContext.h"


namespace LN
{
	
	OpenGLContext::OpenGLContext(GLFWwindow* window) :m_window(window)
	{
		LN_CORE_ASSERT(m_window, "window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_window);
		// ≥ı ºªØGLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LN_CORE_ERROR("Failed to initialize GLAD !");
		}

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_window);
	}

}