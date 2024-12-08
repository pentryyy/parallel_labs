#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "src/BlockMatrix.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DenseMatrix<int> matrix1;
    matrix1.importFromXML("dense_matrix_1.xml");
    matrix1.print();

    DenseMatrix<int> matrix2;
    matrix2.importFromXML("dense_matrix_2.xml");
    matrix2.print();

    DenseMatrix<int> matrixPlus       = matrix1 + matrix2;
    DenseMatrix<int> matrixMinus      = matrix2 - matrix1;
    DenseMatrix<int> matrixMultiply   = matrix1 * matrix2;
    DenseMatrix<int> transposedMatrix = matrix2.transpose();
    DenseMatrix<int> scaledMatrix     = matrix2.scalarMultiplication(2);   

    matrixPlus.exportToXML("dense_matrix_plus.xml");
    matrixMinus.exportToXML("dense_matrix_minus.xml");
    matrixMultiply.exportToXML("dense_matrix_multiply.xml");
    transposedMatrix.exportToXML("dense_matrix_transpose.xml");
    scaledMatrix.exportToXML("dense_matrix_scalar.xml");

    DiagonalMatrix<int> matrix1d;
    matrix1d.importFromXML("diagonal_matrix_1.xml");
    matrix1d.print();

    DiagonalMatrix<int> matrix2d;
    matrix2d.importFromXML("diagonal_matrix_2.xml");
    matrix2d.print();

    DiagonalMatrix<int> matrixPlusD       = matrix1d + matrix2d;
    DiagonalMatrix<int> matrixMinusD      = matrix2d - matrix1d;
    DiagonalMatrix<int> matrixMultiplyD   = matrix1d * matrix2d;
    DiagonalMatrix<int> transposedMatrixD = matrix2d.transpose();
    DiagonalMatrix<int> scaledMatrixD     = matrix2d.scalarMultiplication(3);  

    matrixPlusD.exportToXML("diagonal_matrix_plus.xml");
    matrixMinusD.exportToXML("diagonal_matrix_minus.xml");
    matrixMultiplyD.exportToXML("diagonal_matrix_multiply.xml");
    transposedMatrixD.exportToXML("diagonal_matrix_transpose.xml");
    scaledMatrixD.exportToXML("diagonal_matrix_scalar.xml");

    std::pair<std::size_t, std::size_t> matrixMesh = {5, 4};
    std::pair<std::size_t, std::size_t> matrixSize = {2, 2};

    BlockMatrix<DiagonalMatrix<int>> matrix1b = BlockMatrix<DiagonalMatrix<int>>::Import("block_matrix_1.txt", 
                                                                                         matrixMesh, 
                                                                                         matrixSize);
    BlockMatrix<DiagonalMatrix<int>> matrix2b = BlockMatrix<DiagonalMatrix<int>>::Import("block_matrix_2.txt", 
                                                                                         matrixMesh, 
                                                                                         matrixSize);
    
    matrix1b.Print();
    matrix2b.Print();

    BlockMatrix<DiagonalMatrix<int>> matrixPlusB       = matrix1b + matrix2b;
    BlockMatrix<DiagonalMatrix<int>> matrixMinusB      = matrix2b - matrix1b;
    BlockMatrix<DiagonalMatrix<int>> matrixMultiplyB   = matrix1b * matrix2b;
    BlockMatrix<DiagonalMatrix<int>> transposedMatrixB = matrix2b.transpose();
    BlockMatrix<DiagonalMatrix<int>> scaledMatrixB     = matrix2b.scalarMultiplication(3);

    matrixPlusB.Export("block_matrix_plus.txt");
    matrixMinusB.Export("block_matrix_minus.txt");
    matrixMultiplyB.Export("block_matrix_multiply.txt");
    transposedMatrixB.Export("block_matrix_transpose.txt");
    scaledMatrixB.Export("block_matrix_scalar.txt");
   
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