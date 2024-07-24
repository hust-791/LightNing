#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void Bind()const;
	void UnBind()const;
private:
	unsigned int m_rendererID;
};