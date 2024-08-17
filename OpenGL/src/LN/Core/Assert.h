#pragma once

#include "LN/Core/Log.h"
#include <filesystem>

#ifdef LN_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define LN_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { LN##type##ERROR(msg, __VA_ARGS__); LN_DEBUGBREAK(); } }
#define LN_INTERNAL_ASSERT_WITH_MSG(type, check, ...) LN_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define LN_INTERNAL_ASSERT_NO_MSG(type, check) LN_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", LN_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define LN_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define LN_INTERNAL_ASSERT_GET_MACRO(...) LN_EXPAND_MACRO( LN_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, LN_INTERNAL_ASSERT_WITH_MSG, LN_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define LN_ASSERT(...) LN_EXPAND_MACRO( LN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define LN_CORE_ASSERT(...) LN_EXPAND_MACRO( LN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define LN_ASSERT(...)
#define LN_CORE_ASSERT(...)
#endif
