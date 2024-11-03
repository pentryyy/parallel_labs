@echo off
cd plotting
pyinstaller --onefile --noconsole --add-data "../export/multi_thread_test_log.txt;." main.py
pause