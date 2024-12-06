#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/DiagonalMatrixAlt.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DiagonalMatrixAlt<int> matrix1d;
    matrix1d.importFromXML("diagonal_matrix_1.xml");
    matrix1d.dataOptimization();
    matrix1d.print();

    DiagonalMatrixAlt<int> matrix2d;
    matrix2d.importFromXML("diagonal_matrix_2.xml");
    matrix2d.dataOptimization();
    matrix2d.print();

    DiagonalMatrixAlt<int> matrixPlusD = matrix1d + matrix2d;
    DiagonalMatrixAlt<int> matrixMinusD = matrix2d - matrix1d;
    DiagonalMatrixAlt<int> matrixMultiplyD = matrix1d * matrix2d;
    DiagonalMatrixAlt<int> transposedMatrixD = matrix2d.Transpose();
    DiagonalMatrixAlt<int> scaledMatrixD = matrix2d.ScalarMultiplication(3);  

    matrixPlusD.exportToXML("diagonal_matrix_plus.xml");
    matrixMinusD.exportToXML("diagonal_matrix_minus.xml");
    matrixMultiplyD.exportToXML("diagonal_matrix_multiply.xml");
    transposedMatrixD.exportToXML("diagonal_matrix_transpose.xml");
    scaledMatrixD.exportToXML("diagonal_matrix_scalar.xml");
   
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