#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/DiagonalMatrixAlt.h"
#include "src/DenseMatrixAlt.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DenseMatrixAlt<int> matrix1;
    matrix1.importFromXML("dense_matrix_1.xml");
    matrix1.print();

    DenseMatrixAlt<int> matrix2;
    matrix2.importFromXML("dense_matrix_2.xml");
    matrix2.print();

    DenseMatrixAlt<int> matrixPlus = matrix1 + matrix2;
    DenseMatrixAlt<int> matrixMinus = matrix2 - matrix1;
    DenseMatrixAlt<int> matrixMultiply = matrix1 * matrix2;
    DenseMatrixAlt<int> transposedMatrix = matrix2.Transpose();
    DenseMatrixAlt<int> scaledMatrix = matrix2.ScalarMultiplication(2);   

    matrixPlus.exportToXML("dense_matrix_plus.xml");
    matrixMinus.exportToXML("dense_matrix_minus.xml");
    matrixMultiply.exportToXML("dense_matrix_multiply.xml");
    transposedMatrix.exportToXML("dense_matrix_transpose.xml");
    scaledMatrix.exportToXML("dense_matrix_scalar.xml");

    DiagonalMatrixAlt<int> matrix1d;
    matrix1d.importFromXML("diagonal_matrix_1.xml");
    matrix1d.print();

    DiagonalMatrixAlt<int> matrix2d;
    matrix2d.importFromXML("diagonal_matrix_2.xml");
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