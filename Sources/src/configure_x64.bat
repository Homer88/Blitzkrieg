@echo off
mkdir build_x64 2>nul
cd build_x64
cmake .. -G "Visual Studio 17 2022" -A x64
cd ..
pause