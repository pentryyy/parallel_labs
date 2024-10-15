#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/BlockMatrix.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DenseMatrix<int> matrix1 = DenseMatrix<int>::Import("dense_matrix_1.txt");
    DenseMatrix<int> matrix2 = DenseMatrix<int>::Import("dense_matrix_2.txt");
    
    DenseMatrix<int> matrixPlus = matrix1 + matrix2;
    DenseMatrix<int> matrixMinus = matrix2 - matrix1;
    DenseMatrix<int> matrixMultiply = matrix1 * matrix2;
    DenseMatrix<int> transposedMatrix = matrix2.Transpose();
    DenseMatrix<int> scaledMatrix = matrix2.ScalarMultiplication(2);     

    matrix1.Print();
    matrix2.Print();

    matrixPlus.Export("dense_matrix_plus.txt");
    matrixMinus.Export("dense_matrix_minus.txt");
    matrixMultiply.Export("dense_matrix_multiply.txt");
    transposedMatrix.Export("dense_matrix_transpose.txt");
    scaledMatrix.Export("dense_matrix_scalar.txt");

    DiagonalMatrix<int> matrix1d = DiagonalMatrix<int>::Import("diagonal_matrix_1.txt");
    DiagonalMatrix<int> matrix2d = DiagonalMatrix<int>::Import("diagonal_matrix_2.txt");

    matrix1d.Print();
    matrix2d.Print();

    DiagonalMatrix<int> matrixPlusD = matrix1d + matrix2d;
    DiagonalMatrix<int> matrixMinusD = matrix2d - matrix1d;
    DiagonalMatrix<int> matrixMultiplyD = matrix1d * matrix2d;
    DiagonalMatrix<int> transposedMatrixD = matrix2d.Transpose();
    DiagonalMatrix<int> scaledMatrixD = matrix2d.ScalarMultiplication(3);   

    matrixPlusD.Export("diagonal_matrix_plus.txt");
    matrixMinusD.Export("diagonal_matrix_minus.txt");
    matrixMultiplyD.Export("diagonal_matrix_multiply.txt");
    transposedMatrixD.Export("diagonal_matrix_transpose.txt");
    scaledMatrixD.Export("diagonal_matrix_scalar.txt");

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