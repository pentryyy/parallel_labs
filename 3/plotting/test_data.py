import os
import sys

from time_result import TimeResult

class TestData:
    def __init__(self):
        self.time_result_list          = [] # Список объектов TimeResult
        self.func_name_list            = [] # Список имен функций
        self.metrics_result_dictionary = {} # Словарь результатов метрик

    def get_time_result_list(self):
        return self.time_result_list
    
    def get_func_name_list(self):
        return self.func_name_list
    
    def get_metrics_result_dictionary(self):
        return self.metrics_result_dictionary

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
                        self.func_name_list.append(function_name)
                        self.time_result_list.append(TimeResult(function_name, time_values))
                        function_name = line
                        time_values = []
                       
            if function_name and len(time_values) > 0:
                self.func_name_list.append(function_name)
                self.time_result_list.append(TimeResult(function_name, time_values))

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

        current_function = None
        metrics_dict = {}

        for line in lines:
            line = line.strip()

            if not line:
                continue

            if line in self.get_func_name_list():
                # Сохраняем текущую функцию и её метрики, если уже есть данные
                if current_function and metrics_dict:
                    self.metrics_result_dictionary[current_function] = metrics_dict
                    metrics_dict = {}  # Очищаем словарь для следующей функции
                # Переключаем текущую функцию
                current_function = line
                continue

            if ':::' in line:
                # Если строка содержит метрику, добавляем её
                try:
                    metric_name, metric_value = line.split(':::')
                    metrics_dict[metric_name.strip()] = float(metric_value.strip())
                except ValueError:
                    print(f"Неверный формат метрики: {line}")
                    continue

        # Сохраняем данные последней функции
        if current_function and metrics_dict:
            self.metrics_result_dictionary[current_function] = metrics_dict