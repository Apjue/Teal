# Include Files
## Micropather
Micropather's files are already provided in `/extlibs/src/micropather/` and `micropather.h` is already placed in `/extlibs/include/micropather/`

## Nazara
Teal doesn't use Nazara v0.4, it uses a nightly. You can use any Nazara nightly as long as it is more recent than [`commit 981c5b7`](https://github.com/DigitalPulseSoftware/NazaraEngine/commit/981c5b7e2719091fceb68d62664aff36b81c6838)  
[Link to NazaraEngine's GitHub repository](https://github.com/DigitalPulseSoftware/NazaraEngine) ; Some prebuilts are available [here](https://github.com/S6066/NazaraEngine/releases/tag/v0.4.y)  
- Unzip the archive into `/build/`  
- Use `premake5 --archive-location=<vs2017|gmake2|...> install_nazara` (if you need to update Nazara Engine, you can easily wipe the files with `premake5 --toolset=<...> clean_nazara`)  
or (manual way):  
- Place \[`package/include/`\](`Nazara`|`NDK`) into `/extlibs/include/nazara/`

# Binaries
## Micropather
*Note: Micropather is already built for VS2015 & VS2017. You can skip this step if you use Visual Studio*  
[Link to Micropather's GitHub repository](https://github.com/leethomason/MicroPather) (no need to download any file)  

- Move into `/extlibs/src/micropather/` and launch `premake5` for micropather, then build it  
- Copy the .lib/.a/.pdb files into `/extlibs/lib/<YourIDE>/<YourPlatform>/<LibFolder>/` e.g. `/extlibs/lib/gmake/x86/micropather/`

## Nazara
- If you used `premake5 install_nazara`, do nothing  
else:  
- Download [the prebuilts](https://github.com/S6066/NazaraEngine/releases/tag/v0.4.y) if you use one of the corresponding compilers, else, download the sources and build it.  
- Copy the .lib/.pdb/.a/.so files into `/extlibs/lib/<YourIDE>/<YourPlatform>/nazara/` e.g. `/extlibs/lib/gmake/x86/nazara/`, then copy the .dll files into `/wdirs/<YourPlatform>/`

# Build
Finally, get into `/build/` and use one of the available Build_* scripts in `utility` or launch `premake5` to generate project files for your IDE (try `premake5 --help` to have the list of available actions)  
Then, just build it.