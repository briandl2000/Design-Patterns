workspace "Game Programming Patterns"
    architecture "x64"
    startproject "Command Pattern"

    configurations
    {
        "Debug",
        "Release",
    }
    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] =  "External/GLFW/include"
IncludeDir["Glad"] =  "External/Glad/include"
IncludeDir["ImGui"] = "External/imgui"
IncludeDir["glm"] =   "External/glm"


group "Dependencies"
    include "External/GLFW"
    include "External/Glad"
    include "External/imgui"

group ""

project "Command Pattern"
    location "Command Pattern"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }


    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"