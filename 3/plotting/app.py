import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import mplcursors
import tkinter as tk
from tkinter import filedialog, messagebox
from test_data import TestData
import sys
import numpy as np

class App:
    def __init__(self, root):
        self.__root = root
        self.__root.title("Построение графика масштабирования")
        self.__root.geometry("500x250")
        
        self.__time_file = None
        self.__metrics_file = None

        self.create_widgets()

    def create_widgets(self):
        tk.Label(self.__root, 
                 text="Выберите файлы для построения графика", 
                 font=("Arial", 16, "bold")).pack(pady=10)
        
        # Кнопка для выбора файла метрик
        self.__metrics_label = tk.Label(self.__root, 
                                        text="Путь к файлу метрик не выбран" if self.__metrics_file is None else self.__metrics_file,
                                        font=("Arial", 10))
        self.__metrics_label.pack(pady=5)
        tk.Button(self.__root, 
                  text="Добавить данные метрик", 
                  command=self.__select_metrics_file).pack(pady=5)
        
        # Кнопка для выбора файла времени
        self.__time_label = tk.Label(self.__root, 
                                     text="Путь к файлу времени не выбран" if self.__time_file is None else self.__time_file,
                                     font=("Arial", 10))
        self.__time_label.pack(pady=5)
        tk.Button(self.__root, 
                  text="Добавить данные времени", 
                  command=self.__select_time_file).pack(pady=5)
        tk.Button(self.__root, text="Построить график", command=self.__plot_graph).pack(pady=20)
    
    def __shorten_path(self, file_path, max_length=60):
        if len(file_path) > max_length:
            last_slash_index = file_path.rfind('/')
            
            if last_slash_index != -1:
                return "..." + file_path[last_slash_index:]
            else:
                return "..." + file_path[-max_length:]
        return file_path

    def __select_time_file(self):
        self.__time_file = self.__select_file("с данными времени", "*.txt")
        if self.__time_file:
            self.__time_label.config(text=f"Путь к файлу времени: {self.__shorten_path(self.__time_file)}")
    
    def __select_metrics_file(self):
        self.__metrics_file = self.__select_file("с данными метрик", "*.txt")
        if self.__metrics_file:
            self.__metrics_label.config(text=f"Путь к файлу метрик: {self.__shorten_path(self.__metrics_file)}")
    
    def __select_file(self, file_type_description, file_extension):
        file_path = filedialog.askopenfilename(
            title=f"Выберите файл {file_type_description}",
            filetypes=[(file_type_description, file_extension), ("Все файлы", "*.*")]
        )
        return file_path

    def __plot_graph(self):
        if not self.__time_file or not self.__metrics_file:
            messagebox.showwarning("Недостаточно данных", "Пожалуйста, выберите оба файла перед построением графика.")
            return

        try:
            test_data_list_obj = TestData()
            test_data_list_obj.read_test_data_from_file(self.__time_file)
            test_data_list_obj.read_metrics_from_file(self.__metrics_file)
            
            test_data_list = test_data_list_obj.get_time_result_list()
            metrics_data_dictionary = test_data_list_obj.get_metrics_result_dictionary()

            plt.figure(figsize=(12, 8))
            plt.get_current_fig_manager().set_window_title('Результаты маштабирования')

            colors = ['blue', 'sienna', 'green', 'darkslategrey', 'purple', 'fuchsia', 'darkgoldenrod', 'gray']
            lines = []

            # Проверка на вывод однопоточного лога
            if test_data_list_obj.check_single_thread_log() == True:
                x = np.linspace(1, 2, 400)
                for i, test_data in enumerate(test_data_list):
                    y = np.full_like(x, test_data.get_time_values())
                    line, = plt.plot(x, y, label=test_data.get_function_name(), color=colors[i % len(colors)])
                    lines.append(line)

                plt.xticks([1])
            else:
                for i, test_data in enumerate(test_data_list):
                    line, = plt.plot(test_data.get_time_values(), label=test_data.get_function_name(), color=colors[i % len(colors)])
                    lines.append(line)
                    min_value = min(test_data.get_time_values())
                    min_index = test_data.get_time_values().index(min_value)

                    plt.plot(min_index, min_value, 'ro')
                    plt.axvline(x=min_index, color='red', linestyle='--', linewidth=1)
                plt.xticks(ticks=range(len(test_data_list[0].get_time_values())), labels=range(1, len(test_data_list[0].get_time_values()) + 1))

            plt.title('График маштабирования многопоточных методов')
            plt.xlabel('Количество потоков')
            plt.ylabel('Время выполнения')
            plt.legend()
            plt.grid()

            cursor = mplcursors.cursor(lines, highlight=True)        

            @cursor.connect("add")
            def on_hover(sel):
                selected_line = sel.artist
                for line, test_data in zip(lines, test_data_list):
                    if line != selected_line:
                        continue

                    # Проверка курсора для однопоточного лога
                    if test_data_list_obj.check_single_thread_log() == True:
                        x_index = 1
                    else:
                        x_index = int(round(sel.target[0])) + 1
                    try:
                        metrics_current      = metrics_data_dictionary[test_data.get_function_name()][x_index]
                        metrics_first_thread = metrics_data_dictionary[test_data.get_function_name()][1]
                        
                        sel.annotation.set(text=f"{test_data.get_function_name()} для потоков - {x_index}\n"
                                                f"Минимум: {metrics_current['Минимальное значение']} Маштабирование в {metrics_first_thread['Минимальное значение'] / metrics_current['Минимальное значение']:.2f} раз\n"
                                                f"Максимум: {metrics_current['Максимальное значение']} Маштабирование в {metrics_first_thread['Максимальное значение'] / metrics_current['Максимальное значение']:.2f} раз\n"
                                                f"Среднее: {metrics_current['Среднее значение']} Маштабирование в {metrics_first_thread['Среднее значение'] / metrics_current['Среднее значение']:.2f} раз\n"
                                                f"Медиана: {metrics_current['Медианное значение']} Маштабирование в {metrics_first_thread['Медианное значение'] / metrics_current['Медианное значение']:.2f} раз\n"
                                                f"95-й процентиль: {metrics_current['95-й процентиль']} Маштабирование в {metrics_first_thread['95-й процентиль'] / metrics_current['95-й процентиль']:.2f} раз")

                        sel.annotation.get_bbox_patch().set(fc="white", alpha=0.8)
                    except KeyError:
                        sel.annotation.set(text=f"{test_data.get_function_name()} для потоков - {x_index}\n"
                                                f"Нет данных для этого потока\n")
                        sel.annotation.get_bbox_patch().set(fc="white", alpha=0.8)
                    break
            plt.show()

        except Exception as e:
            messagebox.showerror("Ошибка", f"Произошла ошибка: {str(e)}")
            sys.exit()