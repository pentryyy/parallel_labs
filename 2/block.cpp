#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/BlockMatrix.h"
#include "src/DiagonalMatrix.h"
#include "src/DenseMatrix.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Создание диагональных матриц
    DiagonalMatrix<int> matrix1d;
    matrix1d.importFromXML("diagonal_matrix_1.xml");
    matrix1d.print();

    DiagonalMatrix<int> matrix2d;
    matrix2d.importFromXML("diagonal_matrix_2.xml");
    matrix2d.print();

    // Создание плотных матриц
    DenseMatrix<int> matrix1;
    matrix1.importFromXML("dense_matrix_1.xml");
    matrix1.print();

    DenseMatrix<int> matrix2;
    matrix2.importFromXML("dense_matrix_2.xml");
    matrix2.print();

    // Создание блочной матрицы
    BlockMatrix<DenseMatrix<int>> denseBlockMatrix(2, 2);
    denseBlockMatrix(0, 0) = std::make_shared<DenseMatrix<int>>(matrix1);
    denseBlockMatrix(0, 1) = std::make_shared<DenseMatrix<int>>(matrix2);
    denseBlockMatrix(1, 0) = std::make_shared<DenseMatrix<int>>(matrix1);
    denseBlockMatrix(1, 1) = std::make_shared<DenseMatrix<int>>(matrix2);

    std::cout << "Плотная блочная матрица:\n";
    denseBlockMatrix.print();

    // Создание блочной матрицы с диагональными матрицами
    BlockMatrix<DiagonalMatrix<int>> diagonalBlockMatrix1(2, 2);
    diagonalBlockMatrix1(0, 0) = std::make_shared<DiagonalMatrix<int>>(matrix1d);
    diagonalBlockMatrix1(1, 1) = std::make_shared<DiagonalMatrix<int>>(matrix2d);

    BlockMatrix<DiagonalMatrix<int>> diagonalBlockMatrix2(2, 2);
    diagonalBlockMatrix2(0, 0) = std::make_shared<DiagonalMatrix<int>>(matrix1d);
    diagonalBlockMatrix2(1, 1) = std::make_shared<DiagonalMatrix<int>>(matrix2d);

    std::cout << "Диагональная блочная матрица:\n";
    diagonalBlockMatrix1.print();

    // Пример сложения блочных матриц
    auto sumMatrix = diagonalBlockMatrix1 + diagonalBlockMatrix2;
    std::cout << "Sum of Block Matrices:" << std::endl;
    sumMatrix.print();

    // Пример вычитания блочных матриц
    auto diffMatrix = diagonalBlockMatrix1 - diagonalBlockMatrix2;
    std::cout << "Difference of Block Matrices:" << std::endl;
    diffMatrix.print();

    // Пример умножения блочных матриц
    auto productMatrix = diagonalBlockMatrix1 * diagonalBlockMatrix2;
    std::cout << "Product of Block Matrices:" << std::endl;
    productMatrix.print();

    // Пример транспонирования матрицы
    auto transposedMatrix = diagonalBlockMatrix1.transpose();
    std::cout << "Transposed Block Matrix:" << std::endl;
    transposedMatrix.print();

    // Пример скалярного умножения
    int scalar = 2;
    auto scalarProductMatrix = diagonalBlockMatrix1.scalarMultiplication(scalar);
    std::cout << "Scalar Multiplication of Block Matrix by " << scalar << ":" << std::endl;
    scalarProductMatrix.print();

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