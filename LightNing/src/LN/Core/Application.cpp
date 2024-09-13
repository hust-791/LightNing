#include "stdafx.h"
#include "LN/Core/Window.h"
#include "LN/Event/Event.h"
#include "LN/Renderer/RenderCommand.h"
#include "LN/Core/Layer.h"
#include "LN/ImGui/ImGuiLayer.h"
#include "LN/Renderer/Camera.h"
#include "TestBase.h"
#include "Application.h"

namespace LN
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        LN_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(LN_EVENT_BIND_FUNC(Application::OnEvent));
        m_Window->SetVSync(true);

        m_ImGuiLayer =new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

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

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();

    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverLayer(layer);
        layer->OnAttach();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(0);
            }

            m_ImGuiLayer->Begin();
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }

            m_ImGuiLayer->End();


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
