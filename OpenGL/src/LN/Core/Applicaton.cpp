#include "stdafx.h"
#include "LN/Core/Window.h"
#include "LN/Event/Event.h"

#include "test/TestBase.h"
#include "Applicaton.h"


namespace LN
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        LN_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(LN_EVENT_BIND_FUNC(Application::OnEvent));

        m_TestMenu = TestMenu::Create();
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(LN_EVENT_BIND_FUNC(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(LN_EVENT_BIND_FUNC(Application::OnWindowResize));

        m_TestMenu->GetCurrentTest()->OnEvent(e);
    }

    void Application::Run()
    {
        Renderer renderer;

        while (m_Running)
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            if (m_TestMenu->GetCurrentTest())
            {
                m_TestMenu->GetCurrentTest()->OnUpdata(0.0f);
                m_TestMenu->GetCurrentTest()->OnRender();

                if (m_TestMenu->GetCurrentTest() != m_TestMenu.get() && ImGui::Button("<-"))
                {
                    delete m_TestMenu->GetCurrentTest();
                    m_TestMenu->SetCurrentTest(m_TestMenu.get());
                }
                m_TestMenu->GetCurrentTest()->OnImGuiRender();
            }

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());
            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            m_Window->OnUpdata();
        }
    }

    void Application::Close()
    {
        m_Running = false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        return false;
    }

}
