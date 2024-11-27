import matplotlib.pyplot as plt
import mplcursors
import os
import sys

class TimeResult:
    def __init__(self, function_name, time_values):
        self.__function_name = function_name
        self.__time_values   = time_values

    def get_time_values(self):
        return self.__time_values

    def get_function_name(self):
        return self.__function_name

class TestData:
    def __init__(self):
        self.time_result_list = [] # Список объектов TimeResult
        self.func_name_list   = [] # Список имен функций

    def get_time_result_list(self):
        return self.time_result_list
    
    def get_func_name_list(self):
        return self.func_name_list

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
                
    def read_metrics_from_file(self, filename, function_name):
        metrics_dict = {}

        if getattr(sys, 'frozen', False):
            # Если скрипт запущен в замороженном виде, например, через PyInstaller
            bundle_dir = os.path.abspath(os.path.join(os.getcwd(), "..", ".."))  # Поднимаемся на два уровня вверх
        else:
            # Если скрипт запущен как обычный Python-файл
            bundle_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

        file_path = os.path.join(bundle_dir, "export", filename)
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
            
            current_function = None  # Текущая обрабатываемая функция
            for line in lines:
                stripped_line = line.strip()
                
                if stripped_line in self.get_func_name_list():
                    # Если строка — имя функции, переключаем текущую функцию
                    current_function = stripped_line
                    if current_function == function_name:
                        metrics_dict = {}  # Инициализируем словарь для метрик функции
                    else:
                        # Если текущая функция не совпадает, пропускаем
                        current_function = None
                    continue

                if ':::' in line and current_function == function_name:
                    # Если строка содержит метрику и это нужная функция, добавляем её
                    metric_name, metric_value = line.split(':::')
                    metrics_dict[metric_name.strip()] = float(metric_value.strip())
                
                # Если дошли до конца нужной функции, выходим
                if current_function == function_name and stripped_line not in self.get_func_name_list():
                    continue

        return metrics_dict
 
if __name__ == "__main__":
    try:
        test_data_list_obj = TestData()
        test_data_list_obj.read_test_data_from_file('multi_thread_test_time_log.txt')
        test_data_list = test_data_list_obj.get_time_result_list() 

        plt.figure(figsize=(12, 8))
        plt.get_current_fig_manager().set_window_title('Результаты маштабирования')

        colors = ['blue', 'sienna', 'green', 'darkslategrey', 'purple', 'fuchsia', 'darkgoldenrod', 'gray']

        lines = []  # Для хранения ссылок на графики
        for i, test_data in enumerate(test_data_list):
            line, = plt.plot(test_data.get_time_values(), label=test_data.get_function_name(), color=colors[i % len(colors)])
            lines.append((line, test_data))  # Сохраняем график и данные
            plt.xticks(ticks=range(len(test_data_list[0].get_time_values())), labels=range(1, len(test_data_list[0].get_time_values()) + 1))

            min_value = min(test_data.get_time_values())
            min_index = test_data.get_time_values().index(min_value)

            plt.plot(min_index, min_value, 'ro')
            plt.axvline(x=min_index, color='red', linestyle='--', linewidth=1)

        plt.title('График маштабирования многопоточных методов')
        plt.xlabel('Количество потоков')
        plt.ylabel('Время выполнения')
        
        plt.legend()
        plt.grid()

        # Собираем только объекты графиков (Line2D)
        line_artists = [line for line, _ in lines]

        # Добавляем интерактивность
        cursor = mplcursors.cursor(line_artists, highlight=True)

        @cursor.connect("add")
        def on_hover(sel):
            # Находим соответствующие данные для выбранного графика
            selected_line = sel.artist
            for line, test_data in lines:
                if line == selected_line:
                    metrics = test_data_list_obj.read_metrics_from_file('multi_thread_test_metrics_log.txt', 
                                                                        test_data.get_function_name())
                    sel.annotation.set(text=f"{test_data.get_function_name()}\n"
                                             f"Минимум: {metrics['Минимальное значение']}\n"
                                             f"Максимум: {metrics['Максимальное значение']}\n"
                                             f"Среднее: {metrics['Среднее значение']}\n"
                                             f"Медиана: {metrics['Медианное значение']}\n"
                                             f"95-й процентиль: {metrics['95-й процентиль']}")
                    sel.annotation.get_bbox_patch().set(fc="white", alpha=0.8)
                    break
        plt.show()

    except FileNotFoundError:
        plt.subplots()
        plt.get_current_fig_manager().set_window_title('Результаты маштабирования')
        plt.text(0.5, 0.5, "Построение графика не выполнено, файл не найден", fontsize=16, ha='center', va='center')
        plt.axis("off")

        plt.show()