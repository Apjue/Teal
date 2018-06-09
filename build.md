# Include Files
## Micropather
Micropather's files are already provided [here](https://github.com/S6066/Teal/tree/master/extlibs/src/micropather) and `micropather.h` is already placed in `/extlibs/include/micropather/`

## Nazara
Teal doesn't use Nazara v0.4, it uses an altered nightly.  
Nazara include & source files are located [here](https://github.com/S6066/NazaraEngine/tree/tealnightly). On windows (using msvc15), releases are available [here](https://github.com/S6066/NazaraEngine/releases/tag/v0.4.x)  
Place `Nazara`/`NDK` into `/extlibs/include/nazara/`

# Binaries
## Micropather
*Note: Micropather is already built for VS2015 & VS2017. You can skip this step if you use Visual Studio*

Move into `/extlibs/src/micropather/` and launch `premake5` for micropather, then build it  
Copy the .lib/.a/.pdb files into `/extlibs/lib/<YourIDE>/<YourPlatform>/<LibFolder>` e.g. `/extlibs/lib/gmake/x86/micropather` (check note.md in `extlibs/lib/` for further info)

## Nazara
Download [here](https://github.com/S6066/NazaraEngine/releases/tag/v0.4.x) prebuilts for the altered version of Nazara that Teal uses, download the sources and build it if you don't use one of the corresponding compilers  
Copy the .lib/.a/.pdb files into `/extlibs/lib/<YourIDE>/<YourPlatform>/nazara/` e.g. `/extlibs/lib/gmake/x86/nazara/` (check note.md in `/extlibs/lib/` for further info)  
Copy the .dll/.so files into `/wdirs/<YourPlatform>/`

# Build
Finally, get into the build directory and launch `premake5` to generate project files for your IDE  
Then, just build it.