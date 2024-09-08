#pragma once
#pragma warning(disable:4005)

#include "LNdefine.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace LN
{

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}


#define LN_CORE_TRACE(...)    ::LN::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LN_CORE_INFO(...)     ::LN::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LN_CORE_WARN(...)     ::LN::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LN_CORE_ERROR(...)    ::LN::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LN_CORE_CRITICAL(...) ::LN::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LN_TRACE(...)         ::LN::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LN_INFO(...)          ::LN::Log::GetClientLogger()->info(__VA_ARGS__)
#define LN_WARN(...)          ::LN::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LN_ERROR(...)         ::LN::Log::GetClientLogger()->error(__VA_ARGS__)
#define LN_CRITICAL(...)      ::LN::Log::GetClientLogger()->critical(__VA_ARGS__)
