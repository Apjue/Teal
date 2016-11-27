# Include Files
Download micropather and nazara include files and put them into `extlibs/include/<LibName>` (`LibName` is nazara or micropather)  

# Dependencies
After that, go into `extlibs/src/micropather` and download micropather's files from the github link provided in the `github` file.  
(Only `micropather.h` and `micropather.cpp` are required)  
Launch `premake5` for micropather and build it  
Put the binaries into `extlibs/lib/YourIDE/YourPlatform/LibDir` (check note.txt in `extlibs/lib/` for further info)  

And... do the same for nazara (If you don't want to build it, you can use its nightlies or releases)  

# Build
Finally, get into the build directory and launch `premake5` to generate project files for your IDE  
Then build it.

# Launch
After that, if you try to run it, it will probably crash.  
It's because loading textures failed, you must have this architecture:  

```
SomeFolder
..\data
..\..\img
..\..\addons

..\TealDemo
..\..\TealDemo.exe
..\..\Nazara dynamic libs & others libs
```
(`data` folder is in `wdirs` one)
