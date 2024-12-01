import matplotlib.pyplot as plt
import mplcursors

from test_data import TestData

if __name__ == "__main__":
    try:
        test_data_list_obj = TestData()
        test_data_list_obj.read_test_data_from_file('multi_thread_test_time_log.txt')
        test_data_list_obj.read_metrics_from_file('multi_thread_test_metrics_log.txt')
        
        test_data_list          = test_data_list_obj.get_time_result_list() 
        metrics_data_dictionary = test_data_list_obj.get_metrics_result_dictionary()

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

                    x_index = int(round(sel.target[0])) + 1 # Индекс, выбранной точки
                    metrics = metrics_data_dictionary[test_data.get_function_name()][x_index]
                    
                    sel.annotation.set(text=f"{test_data.get_function_name()} для потоков - {x_index}\n"
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