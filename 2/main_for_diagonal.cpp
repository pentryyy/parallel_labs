#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/DiagonalMatrixAlt.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DiagonalMatrixAlt<int> matrix1;
    matrix1.importFromXML("diagonal_matrix_1.xml");
    matrix1.dataOptimization();

    matrix1.print();
    // matrix1.exportToXML("diagonal_matrix_1.xml");

    DiagonalMatrixAlt<int> matrix2;
    matrix2.importFromXML("diagonal_matrix_2.xml");
    matrix2.dataOptimization();

    matrix2.print();
    // matrix2.exportToXML("diagonal_matrix_2.xml");
   
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