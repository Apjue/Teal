# Teal [![license](https://img.shields.io/badge/license-MIT-blue.svg?style=plastic)](https://github.com/S6066/Teal/blob/master/LICENSE)

Teal is a C++14 RPG, scriptable in Lua.   
It uses a nightly of [Nazara Engine](https://github.com/DigitalPulseSoftware/NazaraEngine), and an altered version of [micropather](https://github.com/leethomason/MicroPather) ([altered source link](https://github.com/S6066/Teal/tree/master/extlibs/src/micropather))

# Build
Platform       | Status          | Nightlies
-------------- | --------------- | ------------------
[Windows (MSVC)](https://ci.appveyor.com/project/S6066/teal) | [![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/github/S6066/Teal?branch=luacomponents&svg=true)](https://ci.appveyor.com/project/S6066/teal) | Release [x64](https://ci.appveyor.com/api/projects/S6066/Teal/artifacts/build%2Fresult%2FTeal.7z?branch=luacomponents&job=Environment:+CONFIG=Release,+SUFFIXDLL=.dll,+SUFFIXLIB=.lib,+PLATFORM=x64,+PLATFORMNAME=x64)/[x86](https://ci.appveyor.com/api/projects/S6066/Teal/artifacts/build%2Fresult%2FTeal.7z?branch=luacomponents&job=Environment:+CONFIG=Release,+SUFFIXDLL=.dll,+SUFFIXLIB=.lib,+PLATFORM=x86,+PLATFORMNAME=Win32), Debug [x64](https://ci.appveyor.com/api/projects/S6066/Teal/artifacts/build%2Fresult%2FTeal.7z?branch=luacomponents&job=Environment:+CONFIG=Debug,+SUFFIXDLL=-d.dll,+SUFFIXLIB=-d.lib,+PLATFORM=x64,+PLATFORMNAME=x64)/[x86](https://ci.appveyor.com/api/projects/S6066/Teal/artifacts/build%2Fresult%2FTeal.7z?branch=luacomponents&job=Environment:+CONFIG=Debug,+SUFFIXDLL=-d.dll,+SUFFIXLIB=-d.lib,+PLATFORM=x86,+PLATFORMNAME=Win32)
[Linux (GCC)](https://travis-ci.org/S6066/Teal) | [![Travis CI Build Status](https://travis-ci.org/S6066/Teal.svg?branch=luacomponents)](https://travis-ci.org/S6066/Teal) | [Debug x64](https://github.com/S6066/Teal-binaries/tree/travis-luacomponents-debug-x64), [Release x64](https://github.com/S6066/Teal-binaries/tree/travis-luacomponents-release-x64)

# How to script?
You can script maps using the [Tiled Map Editor](http://mapeditor.org) and [Tiled2Teal](https://github.com/S6066/tiled2teal)  
Items, animations, characters, and skills can also be customized with scripts, but there's no tutorial (yet!)  
You can look into `/wdirs/data/script/` files though, and try to make your own