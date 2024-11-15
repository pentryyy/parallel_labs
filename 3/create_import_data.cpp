#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include <filesystem> // для работы с файловой системой
#include "src/VectorSingleThread.h"
#include "src/VectorMultiThread.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    std::cout << std::string(32, '-') << "Создание файлов для импорта"  <<  std::string(32, '-') << '\n';

    std::string exportFileNameHundredMillion = "vector_import_1e8_data.txt";
    std::string exportFileNameBillion = "vector_import_1e9_data.txt";
    size_t currentThreadCount = std::thread::hardware_concurrency();

    VectorMultiThread<double> vectorMultiThreadHundredMillion(1e8);
    vectorMultiThreadHundredMillion.setThreadCount(currentThreadCount);
    vectorMultiThreadHundredMillion.initializedRandom(1, 999);
    vectorMultiThreadHundredMillion.exportToFile(exportFileNameHundredMillion);

    VectorMultiThread<double> vectorMultiThreadBillion(1e9);
    vectorMultiThreadBillion.setThreadCount(currentThreadCount);
    vectorMultiThreadBillion.initializedRandom(1, 999);
    vectorMultiThreadBillion.exportToFile(exportFileNameBillion);

    auto moveFileToImport = [](const std::string& fileName) {
        try {
            std::filesystem::path currentPath = std::filesystem::current_path();

            std::filesystem::path importPath = currentPath / "import";
            std::filesystem::path exportPath = currentPath / "export" / fileName;

            if (!std::filesystem::exists(importPath)) {
                std::filesystem::create_directories(importPath);
            }

            std::filesystem::path destinationPath = importPath / fileName;

            std::filesystem::rename(exportPath, destinationPath); 
            std::cout << "Файл " + fileName +  " успешно перемещен из export в import \n";            
        } catch(const std::exception& e) {
            std::cout << "Файл " + fileName + " уже перемещен \n";
        }
    };

    moveFileToImport(exportFileNameHundredMillion);
    moveFileToImport(exportFileNameBillion);

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