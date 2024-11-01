#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/VectorSingleThread.h"
#include "src/VectorMultiThread.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    /* Практически переполнение по типу данных 
    у скалярного умножения поэтому Используется 10^7 */

    std::cout << std::string(32, '-') << "Работа однопоточных методов"  <<  std::string(32, '-') << '\n';
    
    VectorSingleThread<int> vectorSingleThread(1e7);
    // vectorSingleThread.initializedConst(52);
    // vectorSingleThread.initializedRandom(0, 255);
    vectorSingleThread.importFromFile("vector_import_data.txt");
    std::cout << "Минимальное значение: "          << vectorSingleThread.minimumValue() << '\n';
    std::cout << "Индекс минимального значения: "  << vectorSingleThread.minimumIndexByValue() << '\n';
    std::cout << "Максимальное значение: "         << vectorSingleThread.maximumValue() << '\n';
    std::cout << "Индекс максимального значения: " << vectorSingleThread.maximumIndexByValue() << '\n';
    std::cout << "Сумма значений: "                << vectorSingleThread.sumValue() << '\n';
    std::cout << "Среднее значение: "              << vectorSingleThread.avgValue() << '\n';
    std::cout << "Эвклидова норма: "               << vectorSingleThread.euclidMonheton() << '\n';
    std::cout << "Скалярное произведение: "        << vectorSingleThread.scalarMultiply(vectorSingleThread) << '\n';
    vectorSingleThread.exportToFile("vector_single_thread_export.txt");

    std::cout << std::string(32, '-') << "Работа многопоточных методов" <<  std::string(31, '-') << '\n';

    VectorMultiThread<int> vectorMultiThread(1e7);
    vectorMultiThread.setThreadCount(6);
    // vectorMultiThread.initializedConst(52);
    // vectorMultiThread.initializedRandom(0, 255);
    vectorMultiThread.importFromFile("vector_import_data.txt");
    std::cout << "Минимальное значение: "          << vectorMultiThread.minimumValue() << '\n';
    std::cout << "Индекс минимального значения: "  << vectorMultiThread.minimumIndexByValue() << '\n';
    std::cout << "Максимальное значение: "         << vectorMultiThread.maximumValue() << '\n';
    std::cout << "Индекс максимального значения: " << vectorMultiThread.maximumIndexByValue() << '\n';
    std::cout << "Сумма значений: "                << vectorMultiThread.sumValue() << '\n';
    std::cout << "Среднее значение: "              << vectorMultiThread.avgValue() << '\n';
    std::cout << "Эвклидова норма: "               << vectorMultiThread.euclidMonheton() << '\n';
    std::cout << "Скалярное произведение: "        << vectorMultiThread.scalarMultiply(vectorMultiThread) << '\n';
    vectorMultiThread.exportToFile("vector_multi_thread_export.txt");

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