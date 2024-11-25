#include <thread>
#include <chrono>  // Для замера времени
#include <functional> // Для приема функции в качестве аргумента
#include "ConfigReader.h"
#include "HasSetThreadCount.h" // Структура для проверки наличия метода в классе

template <typename VectorT>
class PerformanceTest {
private:
    VectorT vectorT = VectorT(1); // Размерность вектора  по умолчанию 1
    ConfigReader configReader;
public:
    PerformanceTest(VectorT& vectorT, const ConfigReader& configReader) :
        configReader(configReader) {
        this->vectorT.resize(configReader.getVectorSize());
        this->vectorT = vectorT;
    }

    void runTimeTest() {
        int threadCountForTest;
        std::string fileName = configReader.getFileForTestTime();
        
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для записи: " + fileName);
        }

        if (configReader.getNumberOfThreads() == 0) {
            threadCountForTest = std::thread::hardware_concurrency();
        } else {
            threadCountForTest = configReader.getNumberOfThreads();
        }

        // Лямбда для измерения времени теста
        auto measureTime = [this, &file, &threadCountForTest](const std::function<void()>& func, const std::string& funcName) {
            static bool firstCall = true;
            if (!firstCall) { 
                file << '\n'; 
            } else { 
                firstCall = false; 
            }

            file << funcName;
        
            for (int i = 1; i <= threadCountForTest; ++i) {
                float threadSum = 0; // Сумма времени для текущего потока
                for (int j = 1; j <= configReader.getNumberOfTests(); ++j) {
                    /* 
                    Метод setThreadCount() выполняется только в том случае 
                    если он есть в текущем VectorT классе
                    */
                    if constexpr (HasSetThreadCount<VectorT>::value) {
                        vectorT.setThreadCount(i);
                    } else {
                        threadCountForTest = 1;
                    }

                    auto startTime = std::chrono::high_resolution_clock::now();
                    func(); // Выполняем переданную функцию
                    auto endTime = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<double> elapsed = endTime - startTime;
                    threadSum += elapsed.count(); // Суммируем время для текущего потока
                }

                // Вычисляем среднее для текущего потока
                float avgForThread = threadSum / configReader.getNumberOfTests();
                file << '\n' << threadSum / configReader.getNumberOfTests();
            }
        };

        // Выполняем тесты
        measureTime([this]() { vectorT.minimumValue(); }, "minimumValue");
        measureTime([this]() { vectorT.minimumIndexByValue(); }, "minimumIndexByValue");
        measureTime([this]() { vectorT.maximumValue(); }, "maximumValue");
        measureTime([this]() { vectorT.maximumIndexByValue(); }, "maximumIndexByValue");
        measureTime([this]() { vectorT.sumValue(); }, "sumValue");
        measureTime([this]() { vectorT.avgValue(); }, "avgValue"); 
        measureTime([this]() { vectorT.euclidMonheton(); }, "euclidMonheton");
        measureTime([this]() { vectorT.scalarMultiply(vectorT); }, "scalarMultiply");
    
        file.close();
        std::cout << "Данные времени тестирования успешно созданы: " << fileName << '\n';
    }
};