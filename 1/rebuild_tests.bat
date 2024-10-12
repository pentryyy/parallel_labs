@echo off
cd app
rmdir /S /Q build
cmake -S . -B build
cmake --build build
pause