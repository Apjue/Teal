local platformData

if (os.is64bit()) then
    platformData = {"x64", "x86"}
else
    platformData = {"x86", "x64"}
end

local action = _ACTION

local rootFolder = "../.."

workspace "Teal"
    configurations { "Debug", "Release" }
    platforms(platformData)

    filter "platforms:x86"
        architecture "x32"

    filter "platforms:x64"
        architecture "x86_64"

    filter {}

project "TealDemo"
    kind "ConsoleApp"
    language "C++"
    targetdir "%{cfg.buildcfg}/%{cfg.platform}/"
    buildoutputs { action }

    files
    {
        rootFolder .. "/include/**.hpp",
        rootFolder .. "/include/**.inl",
        rootFolder .. "/src/**.cpp"
    }

    libdirs
    { 
        rootFolder .. "/extlibs/lib/" .. action ..  "/%{cfg.platform}/micropather/",
        rootFolder .. "/extlibs/lib/" .. action ..  "/%{cfg.platform}/nazara/"
    }

    includedirs
    {
        rootFolder .. "/extlibs/include/micropather/",
        rootFolder .. "/extlibs/include/nazara/",
        rootFolder .. "/include/"
    }

    flags { "C++14" }

    filter "action:vs*"
        defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" } -- Used to suppress some errors

    filter "configurations:Debug"
        defines { "TEAL_DEBUG", "NAZARA_DEBUG" }
        links { "NazaraCore-d", "NazaraGraphics-d", "NazaraRenderer-d", "NazaraUtility-d", "NazaraSDK-d", "NazaraLua-d", "micropather-d" }
        flags { "symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        links { "NazaraCore", "NazaraGraphics", "NazaraRenderer", "NazaraUtility", "NazaraSDK", "NazaraLua", "micropather" }
        optimize "On"