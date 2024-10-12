#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/DenseMatrix.h"
#include "src/DiagonalMatrix.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DenseMatrix<int> matrix1(3, 3);

    matrix1(0, 0) = 3; matrix1(0, 1) = 20; matrix1(0, 2) = 43;
    matrix1(1, 0) = 33; matrix1(1, 1) = 95; matrix1(1, 2) = 36;
    matrix1(2, 0) = 23; matrix1(2, 1) = 3; matrix1(2, 2) = 5;

    matrix1.Export("dense_matrix_export.txt");
    matrix1.Print();

    DenseMatrix<int> matrix2 = DenseMatrix<int>::Import("dense_matrix_import.txt");
    matrix2.Print();
    
    DenseMatrix<int> matrixPlus = matrix1 + matrix2;
    DenseMatrix<int> matrixMinus = matrix1 - matrix2;
    DenseMatrix<int> matrixMultiply = matrix1 * matrix2;
    
    DenseMatrix<int> transposedMatrix = matrix2.Transpose();
    DenseMatrix<int> scaledMatrix = matrix2.ScalarMultiplication(2);     

    matrixPlus.Print();
    matrixMinus.Print();
    matrixMultiply.Print();

    transposedMatrix.Print();
    scaledMatrix.Print();

    DiagonalMatrix<int> matrix1d(3);
    matrix1d(0, 0) = 13;
    matrix1d(1, 1) = 51;
    matrix1d(2, 2) = 24;

    matrix1d.Export("diagonal_matrix_export.txt");
    matrix1d.Print();

    DiagonalMatrix<int> matrix2d = DiagonalMatrix<int>::Import("diagonal_matrix_import.txt");
    matrix2d.Print();
  
    DiagonalMatrix<int> matrixPlusD = matrix1d + matrix2d;
    DiagonalMatrix<int> matrixMinusD = matrix1d - matrix2d;
    DiagonalMatrix<int> matrixMultiplyD = matrix1d * matrix2d;
 
    DiagonalMatrix<int> transposedMatrixD = matrix2d.Transpose();
    DiagonalMatrix<int> scaledMatrixD = matrix2d.ScalarMultiplication(2);   

    matrixPlusD.Print();
    matrixMinusD.Print();
    matrixMultiplyD.Print();

    transposedMatrixD.Print();
    scaledMatrixD.Print();

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