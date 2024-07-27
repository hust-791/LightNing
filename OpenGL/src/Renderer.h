#pragma once
class VertexArray;
class IndexBuffer;
class Shader;

#define ASSERT(x) if(!(x))__debugbreak();
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x,__FILE__,__LINE__))


void GLClearError();
bool GLLogCall(const char* func, const char* fileName, const int line);

class Renderer
{
public:
	void Draw(VertexArray& va, IndexBuffer& ib, unsigned int drawModel, Shader& shader);
	void Clear();
};
