# Include Files
Download micropather and Nazara include files [here](http://teal.esy.es)* (or [here](https://nazara.digitalpulsesoftware.net/files/) for Nazara) and put them into `extlibs/include/<LibName>` (`LibName` is nazara or micropather)  

# Dependencies
After that, go into `extlibs/src/micropather` and download micropather's files from http://teal.esy.es.  
(Only `micropather.h` and `micropather.cpp` are required)  
Launch `premake5` for micropather and build it  
Put the binaries into `extlibs/lib/YourIDE/YourPlatform/LibDir` (check note.txt in `extlibs/lib/` for further info)  

And... do the same for nazara (If you don't want to build it, you can use its nightlies or releases)  
(Teal currently uses Nazara 0.3)

# Build
Finally, get into the build directory and launch `premake5` to generate project files for your IDE  
Then build it.

*You should download micropather's include file from here because it is altered to compile with Teal.  
 If you still want to download from github, comment line 39 `#define GRINLIZ_NO_STL` in micropather.h