import os
import sys
import re

from time_result import TimeResult

class TestData:
    def __init__(self):
        self.__time_result_list          = [] # Список объектов TimeResult
        self.__func_name_list            = [] # Список имен функций
        self.__metrics_result_dictionary = {} # Словарь результатов метрик

    def get_time_result_list(self):
        return self.__time_result_list
    
    def get_func_name_list(self):
        return self.__func_name_list
    
    def get_metrics_result_dictionary(self):
        return self.__metrics_result_dictionary
    
    def __is_number(self, value):
        return bool(re.match(r"^\d+$", value))

    def read_test_data_from_file(self, filename):
        if getattr(sys, 'frozen', False):
            # Если скрипт запущен в замороженном виде, например, через PyInstaller
            bundle_dir = os.path.abspath(os.path.join(os.getcwd(), "..", ".."))  # Поднимаемся на два уровня вверх
        else:
            # Если скрипт запущен как обычный Python-файл
            bundle_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

        file_path = os.path.join(bundle_dir, "export", filename)
        with open(file_path, 'r') as file:
            lines = file.readlines()
            function_name = None
            time_values = []
            
            for line in lines:
                line = line.strip()

                if not line:
                    continue

                if function_name is None:
                    function_name = line
                else:
                    # Если строка не значение, то добавлем его в список и начинаем сначала
                    try:
                        time_values.append(float(line))
                    except ValueError:
                        self.__func_name_list.append(function_name)
                        self.__time_result_list.append(TimeResult(function_name, time_values))
                        function_name = line
                        time_values = []
                       
            if function_name and len(time_values) > 0:
                self.__func_name_list.append(function_name)
                self.__time_result_list.append(TimeResult(function_name, time_values))

    def read_metrics_from_file(self, filename):
        if getattr(sys, 'frozen', False):
            # Если скрипт запущен в замороженном виде, например, через PyInstaller
            bundle_dir = os.path.abspath(os.path.join(os.getcwd(), "..", ".."))  # Поднимаемся на два уровня вверх
        else:
            # Если скрипт запущен как обычный Python-файл
            bundle_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

        file_path = os.path.join(bundle_dir, "export", filename)
        try:
            with open(file_path, 'r', encoding='utf-8') as file:
                lines = file.readlines()
        except FileNotFoundError:
            print(f"Файл {file_path} не найден.")
            return

        current_function      = None
        current_thread_number = None

        for line in lines:
            line = line.strip()

            if not line:
                continue

            # Установка текущей функции
            if line in self.get_func_name_list():
                current_function = line
                if current_function not in self.__metrics_result_dictionary:
                    self.__metrics_result_dictionary[current_function] = {}
                continue
            
            # Установка текущего номера потока
            if self.__is_number(line):
                current_thread_number = int(line)
                continue

            if ':::' in line:
                try:
                    metric_name, metric_value = line.split(':::')
                    metric_name = metric_name.strip()
                    metric_value = float(metric_value.strip())

                    # Проверяем, есть ли уже такой же номер потока
                    if current_thread_number not in self.__metrics_result_dictionary[current_function]:
                        self.__metrics_result_dictionary[current_function][current_thread_number] = {}

                    # Записываем данные метрик для текущего номера потока
                    self.__metrics_result_dictionary[current_function][current_thread_number][metric_name] = metric_value
                except ValueError:
                    print(f"Неверный формат метрики: {line}")
                    continue