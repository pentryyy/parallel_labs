#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/VectorSingleThread.h"
#include "src/VectorMultiThread.h"

// Метод для выбора способа иницализации
template <typename VectorType>
void performAction(VectorType& vector, int choice, const std::string& filename) {
    switch (choice) {
        case 1:
            std::cout << "Выбрана инициализация константами \n";
            vector.initializedConst(52);
            break;
        case 2:
            std::cout << "Выбрана инициализация случайными числами \n";
            vector.initializedRandom(1, 255);
            break;
        case 3:
            std::cout << "Выбран импорт с файла - " + filename + '\n';
            vector.importFromFile(filename);
            break;
        default:
            std::cout << "Неверный выбор \n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    /* Примеры работы с импортом и экспортом:
    vectorClassThread.importFromFile("vector_import.txt"); импорт в объект
    vectorClassThread.exportToFile("vector_export.txt"); импорт из объекта
    */

    /* Создано по два объекта каждого класса (SingleThread и MultiThread), 
    чтобы во втором объекте хранить обратные значения для скалярного произведения
    */
    std::cout << "На пк больше 24гб ОЗУ? (y/n) \n";
    
    size_t sizeOfVector;
    std::string filename;
    char inputChar;
    
    std::cin >> inputChar;
    while(true) {
        if (inputChar == 'y' || inputChar == 'Y') {
            sizeOfVector = 1e9;
            filename = "vector_import_1e9_data.txt";
            break;
        } else if (inputChar == 'n' || inputChar == 'N') {
            sizeOfVector = 1e8;
            filename = "vector_import_1e8_data.txt";
            break;
        } else {
            std::cout << "Неверное значение \n";
            std::cin >> inputChar;
        }
    }
    std::cout << "Используется размерность вектора - " + std::to_string(sizeOfVector) + '\n';
   
    std::cout << "Выберете метод для инициализации объекта: \n"
              << "1-Инициализация константами \n"
              << "2-Инициализация случайными числами \n"
              << "3-Импорт с файла \n";

    while (true){
        std::cin >> inputChar;

        // (int)(inputChar - '0') это преобразование строки в число
        if ((int)(inputChar - '0') >= 1 && (int)(inputChar - '0') <= 3) {
            break;
        } else{
            std::cout << "Неверное значение \n";
        }
    }   

    std::cout << std::string(32, '-') << "Работа однопоточных методов"  <<  std::string(32, '-') << '\n';
    
    VectorSingleThread<double> vectorSingleThread(sizeOfVector);
    VectorSingleThread<double> vectorSingleThreadInverted(sizeOfVector);
    
    performAction(vectorSingleThread, (int)(inputChar - '0'), filename);

    vectorSingleThreadInverted = vectorSingleThread;
    vectorSingleThreadInverted.invertValues();

    std::cout << "Минимальное значение: "          << vectorSingleThread.minimumValue() << '\n';
    std::cout << "Индекс минимального значения: "  << vectorSingleThread.minimumIndexByValue() << '\n';
    std::cout << "Максимальное значение: "         << vectorSingleThread.maximumValue() << '\n';
    std::cout << "Индекс максимального значения: " << vectorSingleThread.maximumIndexByValue() << '\n';
    std::cout << "Сумма значений: "                << vectorSingleThread.sumValue() << '\n';
    std::cout << "Среднее значение: "              << vectorSingleThread.avgValue() << '\n';
    std::cout << "Эвклидова норма: "               << vectorSingleThread.euclidMonheton() << '\n';
    std::cout << "Скалярное произведение: "        << vectorSingleThread.scalarMultiply(vectorSingleThreadInverted) << '\n';
    vectorSingleThread.createTestData("single_thread_test_log.txt");

    std::cout << std::string(32, '-') << "Работа многопоточных методов" <<  std::string(31, '-') << '\n';

    VectorMultiThread<double> vectorMultiThread(sizeOfVector);
    vectorMultiThread.setThreadCount(10);
    VectorMultiThread<double> vectorMultiThreadInverted(sizeOfVector);

    performAction(vectorMultiThread, (int)(inputChar - '0'), filename);

    vectorMultiThreadInverted = vectorMultiThread;
    vectorMultiThreadInverted.invertValues();

    std::cout << "Минимальное значение: "          << vectorMultiThread.minimumValue() << '\n';
    std::cout << "Индекс минимального значения: "  << vectorMultiThread.minimumIndexByValue() << '\n';
    std::cout << "Максимальное значение: "         << vectorMultiThread.maximumValue() << '\n';
    std::cout << "Индекс максимального значения: " << vectorMultiThread.maximumIndexByValue() << '\n';
    std::cout << "Сумма значений: "                << vectorMultiThread.sumValue() << '\n';
    std::cout << "Среднее значение: "              << vectorMultiThread.avgValue() << '\n';
    std::cout << "Эвклидова норма: "               << vectorMultiThread.euclidMonheton() << '\n';
    std::cout << "Скалярное произведение: "        << vectorMultiThread.scalarMultiply(vectorMultiThreadInverted) << '\n';
    vectorMultiThread.createTestData("multi_thread_test_log.txt");

    std::cout << "Нажмите любую клавишу для продолжения... ";

    /* 
    Запись строк в память вместо стандартного вывода (консоли) 
    для корректного отображения кириллицы при запуске main.exe.
    */
    std::ostringstream buffer;
    std::streambuf* originalCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    std::cout.rdbuf(originalCoutBuffer);
    std::string output = buffer.str();

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideOutput = converter.from_bytes(output);

    getch();
    return 0;
}