@echo off
cd plotting
pyinstaller --onefile --noconsole --add-data "../export/*;export" main.py
pause