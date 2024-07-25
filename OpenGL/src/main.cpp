#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#define WIDTH 800.0f
#define HIGTH 600.0f

// ���״̬����
bool isLeftClick = false;
bool firstMouse = true;
float lastX = WIDTH / 2.0;
float lastY = HIGTH / 2.0;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

// ���λ�úͷ���
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);
// ��갴ť�¼�������
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // ��ʼ��¼���λ��������ת
        isLeftClick = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        isLeftClick = false;
        firstMouse = true;
    }
}
// �����λ���¼�������
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

    // ������ת����
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(-yoffset), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));

    modelMatrix = rotateY * rotateX * modelMatrix;
}
// �������¼�������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= (float)yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& path)
{
    std::ifstream stream(path);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}
// ������ɫ������
GLuint compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}
// ������������ɫ������
GLuint createShaderProgram(const std::string& filePath) {

    auto [vShaderSrc, fShaderSrc] = ParseShader(filePath);
    GLuint vertexShader = compileShader(vShaderSrc, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fShaderSrc, GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
// ������
int main() {
    // ��ʼ��GLFW
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // ��������
    GLFWwindow* window = glfwCreateWindow(WIDTH, HIGTH, "OpenGL Cube", NULL, NULL);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // ע������¼�������
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // ������ɫ������
    GLuint shaderProgram = createShaderProgram("res/shaders/Basic.shader");
    GLuint lineShaderProgram = createShaderProgram("res/shaders/Basic.shader");


    GLfloat vertices[] = {
        // ǰ��
        -0.5f, -0.5f,   0.5f,  
         0.5f, -0.5f,   0.5f,  
         0.5f,  0.5f,   0.5f,  
        -0.5f,  0.5f,   0.5f, 
        -0.5f, -0.5f,  -0.5f,  
         0.5f, -0.5f,  -0.5f,  
         0.5f,  0.5f,  -0.5f,  
        -0.5f,  0.5f,  -0.5f, 
    };

    unsigned int indices[] = {
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

    // ���ö��㻺�����Ͷ����������
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 36);


    GLfloat lineVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
    };

    GLuint lineVAO, lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glBindVertexArray(lineVAO);

    glGenBuffers(1, &lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    GLint colorCube = glGetUniformLocation(shaderProgram, "u_Color");
    assert(colorCube != -1);
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    assert(modelLoc != -1);
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    assert(viewLoc != -1);
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    assert(projLoc != -1);

    GLint colorLine = glGetUniformLocation(lineShaderProgram, "u_Color");
    assert(colorLine != -1);
    GLint modelLocLine = glGetUniformLocation(lineShaderProgram, "model");
    assert(modelLocLine != -1);
    GLint viewLocLine = glGetUniformLocation(lineShaderProgram, "view");
    assert(viewLocLine != -1);
    GLint projLocLine = glGetUniformLocation(lineShaderProgram, "projection");
    assert(projLocLine != -1);

    float r = 0.0f;
    float increment = 0.05f;

    // ��ѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);

        // ������ɫ�����͸���ɫ��
        if (r > 1.0f)
            increment = -0.03f;
        else if(r < 0.0f)
            increment =  0.03f;
        r += increment;

        glUniform4f(colorCube, r, 0.3f, 0.8f, 1.0f);
        // ����ģ�;���
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        // ������ͼ����
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // ����ͶӰ����
        glm::mat4 projection = glm::perspective(glm::radians(fov), WIDTH / HIGTH, 0.1f, 100.0f);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // ����������
        va.Bind();
        ib.Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);// 36������������


        glUseProgram(lineShaderProgram);

        glUniform4fv(colorLine, 1, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
        // ����ģ�;���
        glUniformMatrix4fv(modelLocLine, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        // ������ͼ����
        glm::mat4 viewLine = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLocLine, 1, GL_FALSE, glm::value_ptr(viewLine));
        // ����ͶӰ����
        glm::mat4 projectionLine = glm::perspective(glm::radians(fov), WIDTH / HIGTH, 0.1f, 100.0f);
        glUniformMatrix4fv(projLocLine, 1, GL_FALSE, glm::value_ptr(projectionLine));

        glBindVertexArray(lineVAO);
        glDrawArrays(GL_LINES, 0, 24);
        glBindVertexArray(0);

        // ����������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ������Դ
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}