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

    auto matrixPlusB       = diagonalBlockMatrix1 + diagonalBlockMatrix2;
    auto matrixMinusB      = diagonalBlockMatrix1 - diagonalBlockMatrix2;
    auto matrixMultiplyB   = diagonalBlockMatrix1 * diagonalBlockMatrix2;
    auto transposedMatrixB = diagonalBlockMatrix1.transpose();
    auto scaledMatrixB     = diagonalBlockMatrix1.scalarMultiplication(2);
    
    matrixPlusB.exportToDirectory("block_matrix_plus");
    matrixMinusB.exportToDirectory("block_matrix_minus");
    matrixMultiplyB.exportToDirectory("block_matrix_multiply");
    transposedMatrixB.exportToDirectory("block_matrix_transpose");
    scaledMatrixB.exportToDirectory("block_matrix_scalar");

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