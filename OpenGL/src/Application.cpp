#include "stdafx.h"

#define WIDTH 800.0f
#define HIGTH 600.0f

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

// 主函数
int main() 
{
    // 初始化GLFW
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow((int)WIDTH, (int)HIGTH, "OpenGL Cube", NULL, NULL);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // 注册鼠标事件处理器
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    {
        // 创建着色器程序
        Shader shaderCube("res/shaders/Basic.shader");
        Shader shaderLine("res/shaders/Basic.shader");

        GLfloat vertices[] = {
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
        };
        unsigned int indicesCube[] = {
            0,1,2,
            2,3,0,
            1,5,6,
            6,2,1,
            4,0,3,
            3,7,4,
            3,2,6,
            6,7,3,
            4,5,1,
            1,0,4,
            5,4,7,
            7,6,5
        };
        unsigned int indicesLine[] = {
            0,1,
            1,2,
            2,3,
            3,0,
            0,4,
            1,5,
            2,6,
            3,7,
            4,5,
            5,6,
            6,7,
            7,4
        };

        // 配置顶点缓冲对象和顶点数组对象
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ibCube(indicesCube, 36);
        IndexBuffer ibLine(indicesLine, 24);
        //IndexBuffer ibCube(indicesCube, 6);
        //IndexBuffer ibLine(indicesLine, 8);

        Texture texture("res/texture/kun.jpeg");
        texture.Bind();


        float r = 0.0f;
        float increment = 0.05f;

        Renderer renderer;
        // 主循环
        while (!glfwWindowShouldClose(window))
        {
            // 清屏
            renderer.Clear();

            // 更新颜色
            if (r > 1.0f) increment = -0.03f;
            else if (r < 0.0f) increment = 0.03f;
            r += increment;

            //cube
            shaderCube.Bind();
            texture.Bind();
            shaderCube.SetUniform1i("u_Texture", 0);

            shaderCube.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            // 设置模型矩阵
            shaderCube.SetUniformMatrix4fv("model", 1, GL_FALSE, modelMatrix);
            // 设置视图矩阵
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            shaderCube.SetUniformMatrix4fv("view", 1, GL_FALSE, view);
            // 设置投影矩阵
            glm::mat4 projection = glm::perspective(glm::radians(fov), WIDTH / HIGTH, 0.1f, 100.0f);
            shaderCube.SetUniformMatrix4fv("projection", 1, GL_FALSE, projection);
            // DrawCall
            renderer.Draw(va, ibCube, GL_TRIANGLES, shaderCube);


            //Line
            shaderLine.Bind();
            texture.UnBind();

            shaderLine.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
            // 设置模型矩阵
            shaderLine.SetUniformMatrix4fv("model", 1, GL_FALSE, modelMatrix);
            // 设置视图矩阵
            shaderLine.SetUniformMatrix4fv("view", 1, GL_FALSE, view);
            // 设置投影矩阵
            shaderLine.SetUniformMatrix4fv("projection", 1, GL_FALSE, projection);
            // DrawCall
            renderer.Draw(va, ibLine, GL_LINES, shaderLine);

            // 交换缓冲区
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // 清理资源
    glfwTerminate();

    return 0;
}