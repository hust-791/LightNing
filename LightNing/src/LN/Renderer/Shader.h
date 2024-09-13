#pragma once
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string  m_Path;
	std::unordered_map<std::string, int> m_LocationMap;

public:
	Shader(std::string path);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform1i(std::string name, int val);
	void SetUniform1f(std::string name, float val);
	void SetUniform4f(std::string name, float f1, float f2, float f3, float f4);
	void SetUniformMatrix4fv(std::string name, int count, unsigned char transpose, const glm::mat4& mat);

private:
	ShaderProgramSource __ParseShader(const std::string& path);
	unsigned int __CompileShader(const std::string& source, unsigned int type);
	unsigned int __CreateShaderProgram(const std::string& filePath);

	int __GetLocation(std::string name);
};
