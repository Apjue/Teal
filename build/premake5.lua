local platformData
if (os.is64bit()) then
    platformData = {"x64", "x86"}
else
    platformData = {"x86", "x64"}
end

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

    files
    {
        "../include/**.hpp",
        "../include/**.inl",
        "../src/**.cpp"
    }

    local compiler = _ACTION

    if (_ACTION == nil) then
        compiler = "compiler"
    end

    libdirs { "../extlibs/lib/" .. compiler ..  "/%{cfg.platform}/micropather/", "../extlibs/lib/" .. compiler ..  "/%{cfg.platform}/nazara/" }
    includedirs { "../extlibs/include/micropather/", "../extlibs/include/nazara/", "../include/" }
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