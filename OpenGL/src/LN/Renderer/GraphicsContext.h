#pragma once


namespace LN
{

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};

}