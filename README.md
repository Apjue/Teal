# Teal [![license](https://img.shields.io/badge/license-MIT-blue.svg?style=plastic)](https://github.com/S6066/Teal/blob/master/LICENSE)

Teal is an RPG (still in progress)  
It uses an altered version of [Nazara Engine](https://github.com/DigitalPulseSoftware/NazaraEngine) ([altered source link](https://github.com/S6066/NazaraEngine/tree/tealnightly)), and an altered version of [micropather](https://github.com/leethomason/MicroPather) ([altered source link](https://github.com/S6066/Teal/tree/master/extlibs/src/micropather)).

# Build
Platform       | Status          | Nightlies
-------------- | --------------- | ------------------
[Windows (MSVC)](https://ci.appveyor.com/project/S6066/teal) | [![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/github/S6066/Teal?branch=luacomponents&svg=true)](https://ci.appveyor.com/project/S6066/teal) | [Release x64](https://ci.appveyor.com/api/projects/S6066/Teal/artifacts/build%2Fresult%2FTealDemo.7z?branch=luacomponents&job=Environment:+CONFIG=Release,+SUFFIXDLL=.dll,+SUFFIXLIB=.lib,+PLATFORM=x64,+PLATFORMNAME=x64), [Release x86](https://ci.appveyor.com/api/projects/S6066/Teal/artifacts/build%2Fresult%2FTealDemo.7z?branch=luacomponents&job=Environment:+CONFIG=Release,+SUFFIXDLL=.dll,+SUFFIXLIB=.lib,+PLATFORM=x86,+PLATFORMNAME=Win32)
[Linux (GCC)](https://travis-ci.org/S6066/Teal) | [![Travis CI Build Status](https://travis-ci.org/S6066/Teal.svg?branch=luacomponents)](https://travis-ci.org/S6066/Teal) | [Debug x64](https://github.com/S6066/Teal-binaries/tree/travis-luacomponents-debug-x64), [Release x64](https://github.com/S6066/Teal-binaries/tree/travis-luacomponents-release-x64)