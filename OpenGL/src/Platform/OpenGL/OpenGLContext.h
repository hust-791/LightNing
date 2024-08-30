#pragma once


struct GLFWwindow;

namespace LN
{
	class OpenGLContext :public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() override;
		void SwapBuffer() override;

	private:
		GLFWwindow* m_window;
	};

}