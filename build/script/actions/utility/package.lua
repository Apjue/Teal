-- Package Teal

newoption
{
    trigger     = "package-config",
    description = "[package] Specifies the configuration of Teal to be packaged (e.g. debug_x86)",
    default = "release_x64"
}

newoption
{
    trigger     = "package-action",
    description = "Folder in which Teal executable is located (/build/<action>/<config>/<platform>/Teal)"
}

ACTION.name = "package"
ACTION.description = "Package Teal into a package_<config>_<platform> folder"

ACTION.execute = function(self, root)

    local copy_folder
    copy_folder = function(source, destination)
        os.mkdir(destination)

        local folders = os.matchdirs(source .. "/*")
        for k, v in pairs(folders) do
            copy_folder(v, destination .. "/" .. path.getname(v))
        end

        local files = os.matchfiles(source .. "/*")
        for k, v in pairs(files) do
            os.copyfile(v, destination .. "/" .. path.getname(v))
        end
    end

    local package_config = _OPTIONS["package-config"]

    if (not package_config or #package_config == 0) then
        error("No configuration provided (--package-config argument)")
    else
        package_config = string.lower(package_config)

        if (package_config ~= "debug_x86" and package_config ~= "debug_x64" and package_config ~= "release_x86" and package_config ~= "release_x64") then
            error("This configuration is not allowed: " .. package_config)
        end
    end

    local config, platform = string.match(package_config, "([^_]+)_([^_]+)")

    local action = _OPTIONS["package-action"]

    if (not action or #action == 0) then
        local all_folders = os.matchdirs("*")
        local real_folders = {}
        local counter = 1

        for k, v in pairs(all_folders) do
            if (v ~= "script" and v ~= "utility" and not string.find(v, "NazaraEngine")) then
                real_folders[counter] = v
                counter = counter + 1
            end
        end

        if (#real_folders == 1) then
            action = real_folders[1]
        else
            print("Multiple toolsets were found and no action provided (--package-action argument)")

            for k, v in pairs(real_folders) do
                print("  [" .. tostring(k) .. "] " .. v)
            end

            print("  [Q] Quit")
            io.write("Choose: ")

            local answer = string.lower(io.read(1))

            for k, v in pairs(real_folders) do
                if (answer == tostring(k)) then
                    action = v
                    break
                end
            end

            if (answer == "q" or answer == "n") then
                return

            elseif (action and #action > 0) then
                -- do nothing, just exit the if scope
            else
                print("I didn't understand, but assumed you wanted to quit")
                return
            end
        end
    end


    local executableFolder = root .. "/wdirs/" .. platform
    local executable

    if (#os.matchfiles(executableFolder .. "/Teal-debug") == 1) then
        executable = os.matchfiles(executableFolder .. "/Teal-debug")

    elseif (#os.matchfiles(executableFolder .. "/Teal-release") == 1) then
        executable = os.matchfiles(executableFolder .. "/Teal-release")

    else
        local executableMatches = os.matchfiles(executableFolder .. "/Teal-*.*")

        for k, v in pairs(executableMatches) do
            if (path.getextension(v) == ".exe") then
                executable = v
                break
            end
        end

        if (not executable or #executable == 0) then
            error("No executable found in " .. executableFolder)
        end
    end

    -- structure:
    -- package_<config>_<platform>
    -- -- data (from /wdirs/data/)
    -- -- Teal
    -- -- -- Teal-release-x64[.exe] (from /build/<action>/<config>/<platform>/Teal[.exe])
    -- -- -- libraries (from /wdirs/<platform>/)

    if (#os.matchdirs(root .. "/package_" .. config .. "_" .. platform) == 1) then
        os.rmdir(root .. "/package_" .. config .. "_" .. platform)
        print("Warning: Existing folder \"/package_" .. config .. "_" .. platform .. "/\" has been deleted")
    end

    os.mkdir(root .. "/package_" .. config .. "_" .. platform)
    os.mkdir(root .. "/package_" .. config .. "_" .. platform .. "/Teal")

    print("Copying data & assets...")
    copy_folder(root .. "/wdirs/data", root .. "/package_" .. config .. "_" .. platform .. "/data")

    if (config == "debug") then
        print("Copying debug libraries...")

        local linux_libs = os.matchfiles(root .. "/wdirs/" .. platform .. "/*-d.so")
        local windows_libs = os.matchfiles(root .. "/wdirs/" .. platform .. "/*-d.dll")

        for k, v in pairs(linux_libs) do
            os.copyfile(v, root .. "/package_" .. config .. "_" .. platform .. "/Teal/" .. path.getname(v))
        end

        for k, v in pairs(windows_libs) do
            os.copyfile(v, root .. "/package_" .. config .. "_" .. platform .. "/Teal/" .. path.getname(v))
        end

    elseif (config == "release") then
        print("Copying release libraries...")

        local all_libs_linux = os.matchfiles(root .. "/wdirs/" .. platform .. "/*.so")
        local all_libs_windows = os.matchfiles(root .. "/wdirs/" .. platform .. "/*.dll")
        local libs = {}
        local counter = 1

        for k, v in pairs(all_libs_linux) do
            if (not string.find(v, "-d")) then
                libs[counter] = v
                counter = counter + 1
            end
        end

        for k, v in pairs(all_libs_windows) do
            if (not string.find(v, "-d")) then
                libs[counter] = v
                counter = counter + 1
            end
        end

        for k, v in pairs(libs) do
            os.copyfile(v, root .. "/package_" .. config .. "_" .. platform .. "/Teal/" .. path.getname(v))
        end
        
    else
        print("Didn't understand the config (\"" .. config .. "\"); Copying all libraries...")
        
        local linux_libs = os.matchfiles(root .. "/wdirs/" .. platform .. "/*.so")
        local windows_libs = os.matchfiles(root .. "/wdirs/" .. platform .. "/*.dll")

        for k, v in pairs(linux_libs) do
            os.copyfile(v, root .. "/package_" .. config .. "_" .. platform .. "/Teal/" .. path.getname(v))
        end

        for k, v in pairs(windows_libs) do
            os.copyfile(v, root .. "/package_" .. config .. "_" .. platform .. "/Teal/" .. path.getname(v))
        end
    end

    -- Windows libraries
    if (#os.matchfiles(root .. "/wdirs/" .. platform .. "/assimp.dll") == 1) then
        os.copyfile(root .. "/wdirs/" .. platform .. "/assimp.dll", root .. "/package_" .. config .. "_" .. platform .. "/Teal/assimp.dll")
    end

    if (#os.matchfiles(root .. "/wdirs/" .. platform .. "/libsndfile-1.dll") == 1) then
        os.copyfile(root .. "/wdirs/" .. platform .. "/libsndfile-1.dll", root .. "/package_" .. config .. "_" .. platform .. "/Teal/libsndfile-1.dll")
    end
    
    if (#os.matchfiles(root .. "/wdirs/" .. platform .. "/Newton.dll") == 1) then
        os.copyfile(root .. "/wdirs/" .. platform .. "/Newton.dll", root .. "/package_" .. config .. "_" .. platform .. "/Teal/Newton.dll")
    end
    
    if (#os.matchfiles(root .. "/wdirs/" .. platform .. "/soft_oal.dll") == 1) then
        os.copyfile(root .. "/wdirs/" .. platform .. "/soft_oal.dll", root .. "/package_" .. config .. "_" .. platform .. "/Teal/soft_oal.dll")
    end

    print("Copying executable...")
    os.copyfile(executable, root .. "/package_" .. config .. "_" .. platform .. "/Teal/Teal-" .. config .. "-" .. platform .. path.getextension(executable))
end