import matplotlib.pyplot as plt
import os
import sys

class TestData:
    def __init__(self, function_name, values):
        self.function_name = function_name
        self.values = values

def read_data_from_file(filename):
    test_data_list = []

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
        values = []
        
        for line in lines:
            line = line.strip()

            if not line:
                continue

            if function_name is None:
                function_name = line
            else:
                try:
                    values.append(float(line))
                except ValueError:
                    test_data_list.append(TestData(function_name, values))
                    function_name = line
                    values = []

        if function_name and len(values) > 0:
            test_data_list.append(TestData(function_name, values))
    return test_data_list
    
if __name__ == "__main__":
    try:
        test_data_list = read_data_from_file('multi_thread_test_log.txt')

        plt.figure(figsize=(12, 8))
        plt.get_current_fig_manager().set_window_title('Результаты маштабирования')

        colors = ['blue', 'sienna', 'green', 'darkslategrey', 'purple', 'fuchsia', 'darkgoldenrod', 'gray']

        for i, test_data in enumerate(test_data_list):
            plt.plot(test_data.values, label=test_data.function_name, color=colors[i % len(colors)])
            plt.xticks(ticks=range(len(test_data_list[0].values)), labels=range(1, len(test_data_list[0].values) + 1))

            min_value = min(test_data.values)
            min_index = test_data.values.index(min_value)

            plt.plot(min_index, min_value, 'ro')
            plt.axvline(x=min_index, color='red', linestyle='--', linewidth=1)

        plt.title('График маштабирования многопоточных методов')
        plt.xlabel('Количество потоков')
        plt.ylabel('Время выполнения')
        
        plt.legend()
        plt.grid()
        plt.show()
    except FileNotFoundError:
        plt.subplots()
        plt.get_current_fig_manager().set_window_title('Результаты маштабирования')
        plt.text(0.5, 0.5, "Построение графика не выполнено, файл не найден", fontsize=16, ha='center', va='center')
        plt.axis("off")

        plt.show()