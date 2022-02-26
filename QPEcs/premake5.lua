project "QPEcs"
location ""
kind "StaticLib"
language "C++"
cppdialect "C++20"
staticruntime "On"

targetdir ("../Bin/" .. outputdir .. "/%{prj.name}")
objdir ("../Temp/" .. outputdir .. "/%{prj.name}")

pchheader "qpecspch.h"
pchsource "Source/qpecspch.cpp"

files
{
    "Source/**.h",
    "Source/**.hpp",
    "Source/**.cpp",
    "Source/**.c"
}

defines
{
    "_CRT_SECURE_NO_WARNINGS"
}

includedirs
{
    "Source"
}

-- flags
-- {
--     "fatalwarnings"
-- }

warnings "Default"
externalwarnings "Off"
externalanglebrackets "On"

filter "system:windows"
    systemversion "latest"

    defines
    {
        "QPE_PLATFORM_WINDOWS"
    }

filter "configurations:Debug"
    defines "QPE_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    defines "QPE_RELEASE"
    runtime "Release"
    optimize "On"

filter "configurations:Dist"
    defines "QPE_DIST"
    runtime "Release"
    optimize "On"