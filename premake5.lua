workspace "LightNing"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包含相对解决方案的目录
IncludeDir = {}
IncludeDir["GLFW"] = "LightNing/vendor/GLFW/include"
IncludeDir["GLAD"] = "LightNing/vendor/GLAD/include"
IncludeDir["ImGui"] = "LightNing/vendor/imgui"
IncludeDir["glm"] = "LightNing/vendor/glm"
IncludeDir["stb_image"] = "LightNing/vendor/stb_image"

include "LightNing/vendor/GLFW"
include "LightNing/vendor/GLAD"
include "LightNing/vendor/ImGui"
include "Sandbox"

project "LightNing"
	location "LightNing"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "stdafx.h"
	pchsource "%{prj.name}/src/stdafx.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		
		"%{prj.name}/test/TestBase.h",
		"%{prj.name}/test/TestBase.cpp",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLM_ENABLE_EXPERIMENTAL",
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/test",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib" 
	}

	filter "system:windows"
		systemversion "latest"


	 filter "configurations:Debug"
        defines "LN_DEBUG"
		runtime "Debug"
		symbols "on"

    filter "configurations:Release"
        defines "LN_RELEASE"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
        defines "LN_DIST"
		runtime "Release"
		optimize "on"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"LightNing/vendor/spdlog/include",
		"LightNing/src",
		"LightNing/vendor"
	}

	links
	{
		"LightNing"
	}

	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
        defines "LN_DEBUG"
		runtime "Debug"
		symbols "on"

    filter "configurations:Release"
        defines "LN_RELEASE"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
        defines "LN_DIST"
		runtime "Release"
		optimize "on"
