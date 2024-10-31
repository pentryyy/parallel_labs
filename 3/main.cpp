#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/VectorSingleThread.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    VectorSingleThread<int> vectorSingleThread(1e7);
    // vectorSingleThread.initializedConst(52);
    // vectorSingleThread.initializedRandom(0, 255);
    vectorSingleThread.importFromFile("vector_single_thread_import.txt");
    std::cout << "Минимальное значение: "          << vectorSingleThread.minimumValue() << '\n';
    std::cout << "Индекс минимального значения: "  << vectorSingleThread.minimumIndexByValue() << '\n';
    std::cout << "Максимальное значение: "         << vectorSingleThread.maximumValue() << '\n';
    std::cout << "Индекс максимального значения: " << vectorSingleThread.maximumIndexByValue() << '\n';
    std::cout << "Сумма значений: "                << vectorSingleThread.sumValue() << '\n';
    std::cout << "Среднее значение: "              << vectorSingleThread.avgValue() << '\n';
    std::cout << "Эвклидова норма: "               << vectorSingleThread.euclidMonheton() << '\n';
    std::cout << "Скалярное произведение: "        << vectorSingleThread.scalarMultiply(vectorSingleThread) << '\n'; // Практически переполнение по типу данных
    vectorSingleThread.exportToFile("vector_single_thread_export.txt");

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