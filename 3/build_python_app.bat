@echo off
cd plotting
pyinstaller --onefile --noconsole --add-data "../export/multi_thread_test_time_log.txt;." --add-data "../export/multi_thread_test_metrics_log.txt;." main.py
pause