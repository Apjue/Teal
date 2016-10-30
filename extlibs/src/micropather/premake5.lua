local platformData
if (os.is64bit()) then
    platformData = {"x64", "x86"}
else
    platformData = {"x86", "x64"}
end

workspace "Micropather"
    configurations { "Debug", "Release" }
    platforms(platformData)

    filter "platforms:x86"
        architecture "x32"

    filter "platforms:x64"
        architecture "x86_64"

    filter {}

project "micropather"
    kind "StaticLib"
    language "C++"
    targetdir("./")
    files { "micropather.h", "micropather.cpp" }

    filter "configurations:Debug"
        flags { "symbols" }
        buildoutputs { "micropather-d" }

    filter "configurations:Release"
        optimize "On"
        buildoutputs { "micropather" }
