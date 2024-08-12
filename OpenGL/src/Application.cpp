#include "stdafx.h"
#include "ClearColorTest.h"
#include "TextureTest.h"
#include "Event.h"

#define WIDTH 1280.0f
#define HIGTH 720.0f

Test::TestMenu* g_testMenu = nullptr;
bool Running = true;


bool OnExit(KeyPressedEvent& e)
{
    switch (e.GetKeyCode())
    {
        case EnKeyCode::Escape:
        {
            Running = false;
        }break;
    }
    return false;
}

void OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyPressedEvent>([](auto&&... args)->decltype(auto) {return OnExit(std::forward<decltype(args)>(args)...); });

    g_testMenu->GetCurrentTest()->OnEvent(e);
}

using EventCallbackFn = std::function<void(Event&)>;

struct WindowData
{
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
};

int main() 
{
    // 初始化GLFW
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow((int)WIDTH, (int)HIGTH, "OpenGL Center", NULL, NULL);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    g_testMenu = new Test::TestMenu(window);

    WindowData windowData;

    windowData.Title = "OpenGL Center";
    windowData.Height = (int)HIGTH;
    windowData.Width  = (int)WIDTH;

    windowData.EventCallback = OnEvent;

    glfwSetWindowUserPointer(window, &windowData);

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
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

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) 
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMoveEvent event(xpos, ypos);
        data.EventCallback(event);
    });

    glfwSetScrollCallback(window, [](GLFWwindow * window, double xoffset, double yoffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event(xoffset, yoffset);
        data.EventCallback(event);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

    glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;


        g_testMenu->RegisterTest<Test::ClearColorTest>("clear color");
        g_testMenu->RegisterTest<Test::TextureTest>("2D Texture");
        // 主循环
        while (!glfwWindowShouldClose(window) && Running)
        {
            // 清屏
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (Test::TestMenu::GetCurrentTest())
            {
                Test::TestMenu::GetCurrentTest()->OnUpdata(0.0f);
                Test::TestMenu::GetCurrentTest()->OnRender();

                if (Test::TestMenu::GetCurrentTest() != g_testMenu && ImGui::Button("<-"))
                {
                    delete Test::TestMenu::GetCurrentTest();
                    g_testMenu->SetCurrentTest(g_testMenu);
                }
                Test::TestMenu::GetCurrentTest()->OnImGuiRender();
            }

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((int)WIDTH, (int)HIGTH);
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

            // 交换缓冲区
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete g_testMenu;
        if (Test::TestMenu::GetCurrentTest() != g_testMenu)
        {
            delete Test::TestMenu::GetCurrentTest();
        }
    }
    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}