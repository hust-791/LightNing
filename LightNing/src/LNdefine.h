#pragma once


#ifdef LN_DEBUG
	#if defined(LN_PLATFORM_WINDOWS)
		#define LN_DEBUGBREAK() __debugbreak();
	#elif defined(LN_PLATFORM_LINUX)
		#include <signal.h>
		#define LN_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
		#define LN_ENABLE_ASSERTS
#else
	#define LN_DEBUGBREAK()
#endif

#define LN_EXPAND_MACRO(x) x
#define LN_STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << x)


namespace LN
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}



#define ASSERT(x) if(!(x)) LN_DEBUGBREAK()

#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();
bool GLLogCall(const char* func, const char* fileName, const int line);


