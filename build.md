# Include Files
## Micropather
Micropather's files are already provided in `/extlibs/src/micropather/` and `micropather.h` is already placed in `/extlibs/include/micropather/`

## Nazara
Teal doesn't use Nazara v0.4, it uses a nightly. You can use any Nazara nightly as long as it is more recent than [`4491bc9`](https://github.com/DigitalPulseSoftware/NazaraEngine/commit/4491bc9c24ba8a33f59fa479c58e02468e811338)  
[Link to NazaraEngine's GitHub repository](https://github.com/DigitalPulseSoftware/NazaraEngine) ; Some prebuilts are available [here](https://github.com/S6066/NazaraEngine/releases/tag/v0.4.x)  
- Place `Nazara`/`NDK` into `/extlibs/include/nazara/`

# Binaries
## Micropather
*Note: Micropather is already built for VS2015 & VS2017. You can skip this step if you use Visual Studio*  
[Link to Micropather's GitHub repository](https://github.com/leethomason/MicroPather) (no need to download any file)  

- Move into `/extlibs/src/micropather/` and launch `premake5` for micropather, then build it  
- Copy the .lib/.a/.pdb files into `/extlibs/lib/<YourIDE>/<YourPlatform>/<LibFolder>/` e.g. `/extlibs/lib/gmake/x86/micropather/`

## Nazara
- Download [the prebuilts](https://github.com/S6066/NazaraEngine/releases/tag/v0.4.y) if you use one of the corresponding compilers, else, download the sources and build it.  
- Copy the .lib/.pdb/.a/.so files into `/extlibs/lib/<YourIDE>/<YourPlatform>/nazara/` e.g. `/extlibs/lib/gmake/x86/nazara/`, then copy the .dll files into `/wdirs/<YourPlatform>/`

# Build
Finally, get into `/build/` and launch `premake5` to generate project files for your IDE (try `premake5 --help` to have the list of available actions)  
Then, just build it.