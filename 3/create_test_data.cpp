#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/VectorSingleThread.h"
#include "src/VectorMultiThread.h"
#include "src/VectorOpenMP.h"
#include "src/test/PerformanceTest.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::cout << std::string(32, '-') << "Создание данных тестов"  <<  std::string(32, '-') << '\n';

    // --------------------------Однопоточный тест-------------------------------

    ConfigReader singleThreadConfig("singleThreadConfig.xml");
   
    VectorSingleThread<double> singleThreadVector(singleThreadConfig.getVectorSize());
    singleThreadVector.initializedRandom(1, 999);

    PerformanceTest singleThreadPerformanceTest(singleThreadVector, singleThreadConfig);

    singleThreadPerformanceTest.runTimeTest();
    singleThreadPerformanceTest.runDataTest();

    // --------------------------Многопоточный тест------------------------------

    ConfigReader multiThreadConfig("multiThreadConfig.xml");
   
    VectorMultiThread<double> multiThreadVector(multiThreadConfig.getVectorSize());
    multiThreadVector.initializedRandom(1, 999);

    PerformanceTest multiThreadPerformanceTest(multiThreadVector, multiThreadConfig);

    multiThreadPerformanceTest.runTimeTest();
    multiThreadPerformanceTest.runDataTest();

    // --------------------------OpenMP тест-------------------------------------

    ConfigReader openMPConfig("OpenMPConfig.xml");
   
    VectorOpenMP<double> openMPVector(openMPConfig.getVectorSize());
    openMPVector.initializedRandom(1, 999);

    PerformanceTest openMPPerformanceTest(openMPVector, openMPConfig);

    openMPPerformanceTest.runTimeTest();
    openMPPerformanceTest.runDataTest();

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