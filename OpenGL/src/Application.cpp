#include "stdafx.h"
#include "ClearColorTest.h"
#include "TextureTest.h"
#include "Event.h"

#define WIDTH 1280.0f
#define HIGTH 720.0f

// 鼠标状态变量
bool isLeftClick = false;
bool firstMouse = true;
float lastX = WIDTH / 2.0;
float lastY = HIGTH / 2.0;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

// 相机位置和方向
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);

Test::TestMenu* g_testMenu = nullptr;
// 鼠标按钮事件处理器
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // 开始记录鼠标位置用于旋转
        isLeftClick = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        isLeftClick = false;
        firstMouse = true;
    }
}
// 鼠标光标位置事件处理器
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (!isLeftClick)
        return;

    if (firstMouse) 
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }
    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // 计算旋转矩阵
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(-yoffset), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));

    modelMatrix = rotateY * rotateX * modelMatrix;
}
// 鼠标滚轮事件处理器
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= (float)yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void OnEvent(Event& e)
{
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
        while (!glfwWindowShouldClose(window))
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