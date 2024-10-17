@echo off
cd ..
rmdir /S /Q build
cmake -S . -B build
cmake --build build
pause