#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/VectorSingleThread.h"
#include "src/VectorMultiThread.h"
#include "src/PerformanceTest.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    std::cout << std::string(32, '-') << "Создание данных теста"  <<  std::string(32, '-') << '\n';

    ConfigReader config("config.xml");
   
    VectorMultiThread<double> vectorMultiThread(config.getVectorSize());
    vectorMultiThread.initializedRandom(1, 999);

    PerformanceTest performanceTest(vectorMultiThread, config);

    performanceTest.runTimeTest();

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