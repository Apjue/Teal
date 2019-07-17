-- Add custom actions to support more IDEs
dofile("script/actions/codeblocks/codeblocks.lua")
dofile("script/actions/codeblocks/_codeblocks.lua")

local rootFolder = ".."

-- Load Teal's custom actions
local Actions = {}
local actions = os.matchfiles("script/actions/utility/*.lua")

for k, v in pairs(actions) do
    local f, err = loadfile(v)
        if (f) then
            ACTION = {}
            f()
            local actionTable = ACTION
            
            local lowerCaseName = string.lower(actionTable.name)
            Actions[lowerCaseName] = actionTable

            newaction
            {
                trigger     = lowerCaseName,
                description = actionTable.description,
                execute     = function () actionTable:execute(rootFolder) end
            }
        else
            print("Unable to load action file: " .. err)
        end
end
ACTION = nil

if (_ACTION == nil or Actions[_ACTION]) then
    return
end

-- Generate project
local platformData

if (os.is64bit()) then
    platformData = {"x64", "x86"}
else
    platformData = {"x86", "x64"}
end

workspace "Teal"
    configurations { "Debug", "Release" }
    platforms(platformData)
    location(_ACTION)
    startproject "Teal"

    filter "platforms:x86"
        architecture "x86"

    filter "platforms:x64"
        architecture "x86_64"

    filter {}

    if (os.ishost("windows") and string.sub(_ACTION, 0, 2) == "vs") then
        local commandLine = "premake5.exe " .. table.concat(_ARGV, ' ')

        project("Regenerate premake")
            kind "Utility"
            prebuildcommands("cd .. && " .. commandLine)
    end


    project "Teal"
        kind "ConsoleApp"
        language "C++"

        targetdir(rootFolder .. "/wdirs/%{cfg.platform}/")
        debugdir(rootFolder .. "/wdirs/%{cfg.platform}/")
        runpathdirs { rootFolder .. "/wdirs/%{cfg.platform}/" }

        files
        {
            rootFolder .. "/teal/include/**.hpp",
            rootFolder .. "/teal/include/**.inl",
            rootFolder .. "/shared/include/**.hpp",
            rootFolder .. "/shared/include/**.inl",
            rootFolder .. "/teal/src/**.cpp"
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
            rootFolder .. "/teal/include/",
            rootFolder .. "/shared/include/"
        }

        pic "On"
        cppdialect "C++14"
        flags { "RelativeLinks", "MultiProcessorCompile", "UndefinedIdentifiers", "NoMinimalRebuild" }

        filter "action:vs*"
            defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" } -- Used to suppress some errors

        filter "configurations:Debug"
            defines { "TEAL_DEBUG", "NAZARA_DEBUG", "TEAL_SAFE" }
            targetname "Teal-debug"
            links { "NazaraAudio-d", "NazaraNetwork-d", "NazaraNoise-d", "NazaraPhysics2D-d", "NazaraPhysics3D-d", "NazaraCore-d", "NazaraGraphics-d", "NazaraRenderer-d", "NazaraUtility-d", "NazaraSDK-d", "NazaraLua-d", "NazaraPlatform-d", "micropather-d" }
            symbols "on"

        filter "configurations:Release"
            defines { "NDEBUG" }
            targetname "Teal-release"
            links { "NazaraAudio", "NazaraNetwork", "NazaraNoise", "NazaraPhysics2D", "NazaraPhysics3D", "NazaraCore", "NazaraGraphics", "NazaraRenderer", "NazaraUtility", "NazaraSDK", "NazaraLua", "NazaraPlatform", "micropather" }
            optimize "On"

    project "Tiled2Teal"
        kind "ConsoleApp"
        language "C++"

        targetdir(rootFolder .. "/wdirs/%{cfg.platform}/")
        debugdir(rootFolder .. "/wdirs/%{cfg.platform}/")
        runpathdirs { rootFolder .. "/wdirs/%{cfg.platform}/" }

        files
        {
            rootFolder .. "/tiled2teal/include/**.hpp",
            rootFolder .. "/tiled2teal/include/**.inl",
            rootFolder .. "/shared/include/**.hpp",
            rootFolder .. "/shared/include/**.inl",
            rootFolder .. "/tiled2teal/src/**.cpp"
        }

        libdirs
        {
            rootFolder .. "/extlibs/lib/" .. _ACTION ..  "/%{cfg.platform}/nazara/"
        }

        includedirs
        {
            rootFolder .. "/extlibs/include/nazara/",
            rootFolder .. "/tiled2teal/include/",
            rootFolder .. "/shared/include/"
        }

        pic "On"
        cppdialect "C++14"
        flags { "RelativeLinks", "MultiProcessorCompile", "UndefinedIdentifiers", "NoMinimalRebuild" }

        filter "action:vs*"
            defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" } -- Used to suppress some errors

        filter "configurations:Debug"
            defines { "T2T_DEBUG", "NAZARA_DEBUG", "TEAL_SAFE" }
            links { "NazaraCore-d", "NazaraLua-d" }
            targetname "Tiled2Teal-debug"
            symbols "on"

        filter "configurations:Release"
            defines { "NDEBUG" }
            links { "NazaraCore", "NazaraLua" }
            targetname "Tiled2Teal-release"
            optimize "On"
