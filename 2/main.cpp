#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/DiagonalMatrix.h"
#include "src/BlockMatrix.h"

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

    std::vector<int> diagonal{12, 43, 26};
    
    DiagonalMatrix<int> matrix1d(diagonal);
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

    // Создание блочной матрицы с 2x2 блоками
    DenseMatrix<int> block1(2, 2);
    block1(0, 0) = 1; block1(0, 1) = 2;
    block1(1, 0) = 3; block1(1, 1) = 4;

    DenseMatrix<int> block2(2, 2);
    block2(0, 0) = 5; block2(0, 1) = 6;
    block2(1, 0) = 7; block2(1, 1) = 8;

    DenseMatrix<int> block3(2, 2);
    block3(0, 0) = 9; block3(0, 1) = 10;
    block3(1, 0) = 11; block3(1, 1) = 12;

    DenseMatrix<int> block4(2, 2);
    block4(0, 0) = 13; block4(0, 1) = 14;
    block4(1, 0) = 15; block4(1, 1) = 16;

    // Создаем блочную матрицу 2x2 блоков
    std::vector<std::vector<DenseMatrix<int>>> blocks = {
        {block1, block2},
        {block3, block4}
    };
    std::vector<std::size_t> row_block_sizes = {2, 2};
    std::vector<std::size_t> col_block_sizes = {2, 2};

    BlockMatrix<int> matrix1b(blocks, row_block_sizes, col_block_sizes);

    // Экспортируем блочную матрицу
    matrix1b.Export("block_matrix_export.txt");

    // Печатаем первую блочную матрицу
    std::cout << "Matrix 1:" << std::endl;
    matrix1b.Print();

    // BlockMatrix<int> matrix2 = BlockMatrix<int>::Import("block_matrix_import.txt");
    // matrix2.Print();

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