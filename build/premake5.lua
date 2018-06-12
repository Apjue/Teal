-- Premake script

if (_ACTION == nil) then -- Check the arguments...
    return
end

dofile("script/actions/codeblocks/_codeblocks.lua")
dofile("script/actions/codeblocks/codeblocks.lua")

local platformData

if (os.is64bit()) then
    platformData = {"x64", "x86"}
else
    platformData = {"x86", "x64"}
end

local rootFolder = ".."

workspace "Teal"
    configurations { "Debug", "Release" }
    platforms(platformData)
    location(_ACTION)

    filter "platforms:x86"
        architecture "x32"

    filter "platforms:x64"
        architecture "x86_64"

    filter {}

    if (os.ishost("windows")) then
        local commandLine = "premake5.exe " .. table.concat(_ARGV, ' ')

        project("Regenerate premake")
            kind("Utility")
            prebuildcommands("cd .. && " .. commandLine)
    end

    project "TealDemo"
        kind "ConsoleApp"
        language "C++"
        targetdir(_ACTION .. "/%{cfg.buildcfg}/%{cfg.platform}/")
        debugdir(rootFolder .. "/wdirs/%{cfg.platform}/")

        files
        {
            rootFolder .. "/include/**.hpp",
            rootFolder .. "/include/**.inl",
            rootFolder .. "/src/**.cpp"
        }

        libdirs
        {
            rootFolder .. "/extlibs/lib/" .. _ACTION ..  "/%{cfg.platform}/micropather/",
            rootFolder .. "/extlibs/lib/" .. _ACTION ..  "/%{cfg.platform}/nazara/"
        }

        includedirs
        {
            rootFolder .. "/extlibs/include/micropather/",
            rootFolder .. "/extlibs/include/nazara/",
            rootFolder .. "/include/"
        }

        pic "On"
        cppdialect "C++14"
        flags { "RelativeLinks", "MultiProcessorCompile", "UndefinedIdentifiers" }

        filter "action:vs*"
            defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" } -- Used to suppress some errors

        filter "configurations:Debug"
            defines { "TEAL_DEBUG", "NAZARA_DEBUG" }
            links { "NazaraAudio-d", "NazaraNetwork-d", "NazaraNoise-d", "NazaraPhysics2D-d", "NazaraPhysics3D-d", "NazaraCore-d", "NazaraGraphics-d", "NazaraRenderer-d", "NazaraUtility-d", "NazaraSDK-d", "NazaraLua-d", "NazaraPlatform-d", "micropather-d" }
            symbols "on"

        filter "configurations:Release"
            defines { "NDEBUG" }
            links { "NazaraAudio", "NazaraNetwork", "NazaraNoise", "NazaraPhysics2D", "NazaraPhysics3D", "NazaraCore", "NazaraGraphics", "NazaraRenderer", "NazaraUtility", "NazaraSDK", "NazaraLua", "NazaraPlatform", "micropather" }
            optimize "On"
