#pragma once

namespace LN
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader :public Shader
	{
	private:
		unsigned int m_RendererID;
		std::string  m_Path;
		std::string m_Name;
		std::unordered_map<std::string, int> m_LocationMap;

	public:
		OpenGLShader(std::string path);
		~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		void SetInt(std::string name, int val) override;
		void SetIntArray(std::string name, int* val, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		const std::string& GetName() const override;
	private:
		ShaderProgramSource __ParseShader(const std::string& path);
		unsigned int __CompileShader(const std::string& source, unsigned int type);
		unsigned int __CreateShaderProgram(const std::string& filePath);

		int __GetLocation(std::string name);
	};
}

