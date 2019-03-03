-- Install Nazara

newoption
{
    trigger     = "archive",
    description = "[install_nazara] Specifies any keyword to clarify which archive should be used",
}

newoption
{
    trigger     = "lib-toolset",
    description = "[install_nazara] Specifies the directory name where Nazara Engine's files will be copied (/extlibs/lib/<location>/(arch)/nazara)",
}

ACTION.name = "install_nazara"
ACTION.description = "Copy Nazara Engine's files into Teal's folders automatically"

ACTION.execute = function(self, root)

    local install_archive = function(folder)

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


        local matches
        local package = "package/"

        matches = os.matchdirs(folder .. "/package")

        if (#matches == 0) then -- folder *is* the package directory
            package = ""
        end

        -- /build/package/bin/ => /wdirs/(arch)/
        print("Copying binaries into /wdirs/(x86|x64)/...")

        os.rmdir(root .. "/wdirs/x86/")
        os.rmdir(root .. "/wdirs/x64/")

        os.mkdir(root .. "/wdirs/x86/")
        os.mkdir(root .. "/wdirs/x64/")

        matches = os.matchfiles(folder .. "/" .. package .. "bin/x86/*")
        for k, v in pairs(matches) do
            os.copyfile(v, root .. "/wdirs/x86/" .. path.getname(v))
        end

        matches = os.matchfiles(folder .. "/" .. package .. "bin/x64/*")
        for k, v in pairs(matches) do
            os.copyfile(v, root .. "/wdirs/x64/" .. path.getname(v))
        end

        -- /build/package/include/ => /extlibs/include/nazara/
        print("Copying include files into /extlibs/include/nazara/...")

        os.rmdir(root .. "/extlibs/include/nazara")
        os.mkdir(root .. "/extlibs/include/nazara")

        copy_folder(folder .. "/" .. package .. "include/Nazara", root .. "/extlibs/include/nazara/Nazara")
        copy_folder(folder .. "/" .. package .. "include/NDK",    root .. "/extlibs/include/nazara/NDK")

        -- /build/package/lib/(arch)/ => /extlibs/lib/<location>/(arch)/nazara/
        local location = _OPTIONS["lib-toolset"]

        if (not location or #location == 0) then
            -- No location provided, try to determine it automatically
            if (string.find(folder, "msvc14%-")) then
                location = "vs2015"
                print("No location provided (--lib-toolset argument), chose vs2015 because of the msvc14 keyword")

            elseif (string.find(folder, "msvc14.1")) then
                location = "vs2017"
                print("No location provided (--lib-toolset argument), chose vs2017 because of the msvc14.1 keyword")

            elseif (string.find(folder, "gcc") or string.find(folder, "g++") or string.find(folder, "clang")) then
                location = "gmake" -- gmake2?
                print("No location provided (--lib-toolset argument), chose gmake because of the gcc/clang keyword")
            end

            if (not location) then
                error("No archive location provided (--lib-toolset argument), couldn't determine it using the archive's name")
            end
        end


        os.mkdir(root .. "/extlibs/lib/" .. location)

        os.mkdir(root .. "/extlibs/lib/" .. location.. "/x86")
        os.rmdir(root .. "/extlibs/lib/" .. location.. "/x86/nazara")
        os.mkdir(root .. "/extlibs/lib/" .. location.. "/x86/nazara")

        os.mkdir(root .. "/extlibs/lib/" .. location.. "/x64")
        os.rmdir(root .. "/extlibs/lib/" .. location.. "/x64/nazara")
        os.mkdir(root .. "/extlibs/lib/" .. location.. "/x64/nazara")

        matches = os.matchfiles(folder .. "/" .. package .. "lib/x86/*")
        for k, v in pairs(matches) do
            os.copyfile(v, root .. "/extlibs/lib/" .. location .. "/x86/nazara/" .. path.getname(v))
        end

        matches = os.matchfiles(folder .. "/" .. package .. "lib/x64/*")
        for k, v in pairs(matches) do
            os.copyfile(v, root .. "/extlibs/lib/" .. location .. "/x64/nazara/" .. path.getname(v))
        end

        if (#matches > 0) then
            print("Copied libraries into /extlibs/lib/" .. location .. "/(x86|x64)/nazara/")
        else
            print("Copying binaries into /extlibs/lib/" .. location .. "/(x86|x64)/nazara/...")

            matches = os.matchfiles(folder .. "/" .. package .. "bin/x86/*")
            for k, v in pairs(matches) do
                os.copyfile(v, root .. "/extlibs/lib/" .. location .. "/x86/nazara/" .. path.getname(v))
            end

            matches = os.matchfiles(folder .. "/" .. package .. "bin/x64/*")
            for k, v in pairs(matches) do
                os.copyfile(v, root .. "/extlibs/lib/" .. location .. "/x64/nazara/" .. path.getname(v))
            end
        end
    end

    local all_archives = os.matchdirs("Nazara*")
    local archive

    if (#all_archives == 0) then
        error("No archive found (the archive must be a folder with a name starting by \"Nazara\", located in /build/)")

    elseif (#all_archives == 1) then
        archive = all_archives[1]
    end

    if (archive) then
        if (_OPTIONS["archive"]) then
            if (not string.find(archive, _OPTIONS["archive"])) then
                print("Warning: " .. archive .. " is the only archive found but does not match with provided keyword \"" .. _OPTIONS["archive"] .. "\"")
                io.write("Do you want it install it anyway? [Y/n] ")

                local answer = string.lower(io.read(1))

                if (answer == "\n" or answer == "\r\n" or answer == "y") then
                    install_archive(archive)
                    return

                elseif (answer == "n") then
                    return

                else
                    print("I didn't understand, but assumed that was a no")
                    return
                end
            end
        end

        install_archive(archive)
    else
        if (_OPTIONS["archive"]) then
            for k, v in pairs(all_archives) do
                if (string.find(v, _OPTIONS["archive"])) then
                    install_archive(v)
                    return
                end
            end
            error("In the " .. #all_archives .. " archives, none of them contained the keyword \"" .. _OPTIONS["archive"] .. "\"")
        else -- List and ask the user
            print("Multiple archives were found and no keyword provided (--archive argument)")

            for k, v in pairs(all_archives) do
                print("  [" .. tostring(k) .. "] " .. v)
            end

            print("  [Q] Quit")
            io.write("Choose: ")

            local answer = string.lower(io.read(1))

            for k, v in pairs(all_archives) do
                if (answer == tostring(k)) then
                    install_archive(v)
                    return
                end
            end

            if (answer == "q" or answer == "n") then
                return

            else
                print("I didn't understand, but assumed you wanted to quit")
                return
            end
        end
    end
end