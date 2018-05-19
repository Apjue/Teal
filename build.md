# Include Files
## Micropather
Micropather's files are already provided [here](https://github.com/S6066/Teal/tree/master/extlibs/src/micropather) and `micropather.h` is already placed in `/extlibs/include/micropather/`

## Nazara
Nazara include & source files are located [here](https://github.com/S6066/NazaraEngine/tree/tealnightly). On windows (using msvc15), you can run `git lfs pull` to find prebuilts in `/extlibs/bin/`  
Place `Nazara`/`NDK` into `/extlibs/include/nazara/`

# Binaries
## Micropather
*Note: Micropather is already built for VS2015 & VS2017. You can skip this step if you use Visual Studio*

Move into `/extlibs/src/micropather/` and launch `premake5` for micropather, then build it  
Copy the .lib/.a/.pdb files into `/extlibs/lib/<YourIDE>/<YourPlatform>/<LibFolder>` e.g. `/extlibs/lib/gmake/x86/micropather` (check note.md in `extlibs/lib/` for further info)

## Nazara
Download the altered version of Nazara that Teal uses [here](https://github.com/S6066/NazaraEngine/tree/tealnightly), and compile it. There's a prebuilt for VS2017 in `extlibs/bin/`  
Copy the .lib/.a/.pdb files into `/extlibs/lib/<YourIDE>/<YourPlatform>/nazara/` e.g. `/extlibs/lib/gmake/x86/nazara/` (check note.md in `/extlibs/lib/` for further info)  
Copy the .dll/.so files into `/wdirs/<YourPlatform>/`

# Build
Finally, get into the build directory and launch `premake5` to generate project files for your IDE  
Then, just build it.