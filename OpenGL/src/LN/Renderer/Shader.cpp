#include "stdafx.h"

Shader::Shader(std::string path) :m_RendererID(0), m_Path(path)
{
    m_RendererID = __CreateShaderProgram(path);
}

Shader::~Shader()
{
   GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(std::string name, int val)
{
    GLCall(glUniform1i(__GetLocation(name), val));
}

void Shader::SetUniform1f(std::string name, float val)
{
    GLCall(glUniform1f(__GetLocation(name), val));
}

void Shader::SetUniform4f(std::string name, float f1, float f2, float f3, float f4)
{
    GLCall(glUniform4f(__GetLocation(name), f1, f2, f3, f4));
}

void Shader::SetUniformMatrix4fv(std::string name, GLsizei count, GLboolean transpose, const glm::mat4& mat)
{
    GLCall(glUniformMatrix4fv(__GetLocation(name), count, transpose, glm::value_ptr(mat)));
}

int Shader::__GetLocation(std::string name)
{
    if (m_LocationMap.find(name) != m_LocationMap.end())
        return m_LocationMap[name];

    GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
    if (loc == -1)
    {
        std::cout << "Location Warning" << std::endl;
    }

    m_LocationMap[name] = loc;
    return loc;
}

ShaderProgramSource Shader::__ParseShader(const std::string& path)
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

unsigned int Shader::__CompileShader(const std::string& source, unsigned int type)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

unsigned int Shader::__CreateShaderProgram(const std::string& filePath)
{
    auto [vShaderSrc, fShaderSrc] = __ParseShader(filePath);
    GLuint vertexShader = __CompileShader(vShaderSrc, GL_VERTEX_SHADER);
    GLuint fragmentShader = __CompileShader(fShaderSrc, GL_FRAGMENT_SHADER);
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

