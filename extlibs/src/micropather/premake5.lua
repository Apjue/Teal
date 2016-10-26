workspace "Micropather"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }

    filter "platforms:x86"
        architecture "x32"

    filter "platforms:x64"
        architecture "x64"

    filter {}

project "micropather"
    kind "StaticLib"
    language "C++"
    targetdir("../../lib/" .. _ACTION ..  "%{cfg.platform}/micropather/")
    files { "micropather.h", "micropather.cpp" }

    filter "configurations:Debug"
        flags { "symbols" }

    filter "configurations:Release"
        optimize "On"
