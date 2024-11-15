workspace "VISH"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "VISH"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "build/%{cfg.buildcfg}"

    files {"src/**.h", "src/**.cpp", "src/**.c"}

    includedirs
    {
        "$(SolutionDir)vendor/includes/"
    }

    libdirs "$(SolutionDir)vendor/libs/"
    filter "kind:not StaticLib"
        links "glfw3"
        links "opengl32"
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"