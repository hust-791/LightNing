#include "stdafx.h"
#include "LN/Core/Window.h"
#include "LN/Event/Event.h"
#include "WindowsWindow.h"


namespace LN
{
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        LN_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowSetting& set)
    {
        Init(set);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdowm();
    }

    void WindowsWindow::OnUpdata()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    uint32_t WindowsWindow::GetWidth() const
    {
        return 0;
    }

    uint32_t WindowsWindow::GetHeight() const
    {
        return 0;
    }

    void WindowsWindow::SetVSync(bool enable)
    {
        if (enable)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_data.VSync = enable;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_data.VSync;
    }

    void WindowsWindow::Init(const WindowSetting& set)
    {
        m_data.Title = set.Title;
        m_data.Width = set.Width;
        m_data.Height = set.Height;

        LN_CORE_INFO("Creating window {0} ({1}, {2})", set.Title, set.Width, set.Height);

        if (s_GLFWWindowCount == 0)
        {
            int success = glfwInit();
            LN_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        // ´´½¨´°¿Ú
        m_Window = glfwCreateWindow((int)set.Width, (int)set.Height, "OpenGL Center", NULL, NULL);
        ++s_GLFWWindowCount;
        if (!m_Window)
        {
            LN_CORE_ASSERT("Failed to create GLFW m_Window !");
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetWindowUserPointer(m_Window, &m_data);

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMoveEvent event(xpos, ypos);
                data.EventCallback(event);
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event(xoffset, yoffset);
                data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, true);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent event(keycode);
                data.EventCallback(event);
            });
    }

    void WindowsWindow::Shutdowm()
    {
        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }
}
