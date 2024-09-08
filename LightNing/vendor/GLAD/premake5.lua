project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad.h",
		"include/khrplatform.h",
		"src/glad.c"
	}
	includedirs{
		"include" -- Ϊ��glad.cֱ��#include <glad/glad.h>��������#include <include/glad/glad.h>
	}

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"