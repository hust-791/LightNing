#pragma once

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


#define LN_DEBUGBREAK() __debugbreak()
#define LN_EXPAND_MACRO(x) x
#define LN_STRINGIFY_MACRO(x) #x


#define ASSERT(x) if(!(x))__debugbreak();
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x,__FILE__,__LINE__))


void GLClearError();
bool GLLogCall(const char* func, const char* fileName, const int line);

