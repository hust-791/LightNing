#include "stdafx.h"
#include "OpenGLShader.h"


namespace LN
{
    OpenGLShader::OpenGLShader(std::string path) :m_RendererID(0), m_Path(path)
    {
        m_RendererID = __CreateShaderProgram(path);
        m_Name = "shader";
    }
    OpenGLShader::~OpenGLShader()
    {
        GLCall(glDeleteProgram(m_RendererID));
    }
    void OpenGLShader::Bind() const
    {
        GLCall(glUseProgram(m_RendererID));
    }
    void OpenGLShader::UnBind() const
    {
        GLCall(glUseProgram(0));
    }
    void OpenGLShader::SetInt(std::string name, int val)
    {
        GLCall(glUniform1i(__GetLocation(name), val));
    }
    void OpenGLShader::SetIntArray(std::string name, int* val, uint32_t count)
    {
        GLCall(glUniform1iv(__GetLocation(name), count, val));
    }
    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        GLCall(glUniform1f(__GetLocation(name), value));
    }
    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        GLCall(glUniform2f(__GetLocation(name), value.x, value.y));
    }
    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        GLCall(glUniform3f(__GetLocation(name), value.x, value.y, value.z));
    }
    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        GLCall(glUniform4f(__GetLocation(name), value.x, value.y, value.z, value.w));
    }
    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        GLCall(glUniformMatrix4fv(__GetLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
    }
    const std::string& OpenGLShader::GetName() const
    {
        return m_Name;
    }
    ShaderProgramSource OpenGLShader::__ParseShader(const std::string& path)
    {
        std::ifstream stream(path);

        LN_CORE_ASSERT(stream, "can not find shader file!");

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
    unsigned int OpenGLShader::__CompileShader(const std::string& source, unsigned int type)
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
    unsigned int OpenGLShader::__CreateShaderProgram(const std::string& filePath)
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
    int OpenGLShader::__GetLocation(std::string name)
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
}
