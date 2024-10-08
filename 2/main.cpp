#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>
#include "main/DenseMatrix.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    DenseMatrix<int> matrix1(2, 2);
    DenseMatrix<int> matrix2(2, 2);

    matrix1(0, 0) = 1;
    matrix1(0, 1) = 2;
    matrix1(1, 0) = 3;
    matrix1(1, 1) = 4;

    DenseMatrix<int> result = matrix1 + matrix2;

    std::cout << "Результат сложения:\n";
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            std::cout << result(i, j) << " ";
        }
        std::cout << std::endl;
    }

    DenseMatrix<int> multResult = matrix1 * matrix2;

    std::cout << "Результат умножения:\n";
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            std::cout << multResult(i, j) << " ";
        }
        std::cout << std::endl;
    }

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