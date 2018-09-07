-- Clean External Libraries

newoption
{
    trigger     = "toolset",
    description = "[clean_nazara] Specifies the toolset used, to remove files in /extlibs/lib/<toolset>/(arch)/nazara/"
}

ACTION.name = "clean_nazara"
ACTION.description = "Remove Nazara Engine's files in /wdirs/ and /extlibs/"

ACTION.execute = function(self, root)
    -- Remove /wdirs/ files
    print("Cleaning /wdirs/x86/ and /wdirs/x64/...")

    os.rmdir(root .. "/wdirs/x86")
    os.rmdir(root .. "/wdirs/x64")
    os.mkdir(root .. "/wdirs/x86")
    os.mkdir(root .. "/wdirs/x64")

    -- Remove /extlibs/include/nazara/ files
    print("Cleaning /extlibs/include/nazara/...")

    os.rmdir(root .. "/extlibs/include/nazara")
    os.mkdir(root .. "/extlibs/include/nazara")

    -- Remove /extlibs/lib/ files
    local toolset = _OPTIONS["toolset"]

    if (toolset) then
        if (string.find(toolset, '/') or string.find(toolset, '\\')) then
            print("Slashes are not allowed in the toolset...")
            toolset = nil
        end
    end
    
    if (not toolset or #toolset == 0) then
        local toolsets = os.matchdirs(root .. "/extlibs/lib/*")

        if (#toolsets > 1) then
            error("More than one subdirectory was found in the /extlibs/lib/ directory, please use the --toolset argument to clarify which directory should be used")

        elseif (#toolsets == 0) then
            return

        else
            toolset = path.getname(toolsets[1])
            print("No directory was set by the --toolset argument, \"" .. toolset .. "\" will be used")
        end
    end

    print("Cleaning /extlibs/lib/" .. toolset .. "/(x86|x64)/nazara/...")

    os.rmdir(root .. "/extlibs/lib/" .. toolset .. "/x86/nazara")
    os.rmdir(root .. "/extlibs/lib/" .. toolset .. "/x64/nazara")
    os.mkdir(root .. "/extlibs/lib/" .. toolset .. "/x86/nazara")
    os.mkdir(root .. "/extlibs/lib/" .. toolset .. "/x64/nazara")
end