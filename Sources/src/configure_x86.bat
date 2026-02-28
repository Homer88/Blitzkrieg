@echo off
mkdir build_x86 2>nul
cd build_x86
cmake .. -G "Visual Studio 17 2022" -A Win32
cd ..
pause