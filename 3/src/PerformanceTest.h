#include <thread>
#include <chrono>  // Для замера времени
#include <functional> // Для приема функции в качестве аргумента
#include "ConfigReader.h"

template <typename VectorT>
class PerformanceTest {
private:
    VectorT vectorT = VectorT(1); // Размерность вектора  по умолчанию 1
    ConfigReader configReader;
    float executionTime;
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

        // Устанавливаем количество потоков
        if (configReader.getNumberOfThreads() == 0) {
            threadCountForTest = std::thread::hardware_concurrency();
        } else {
            threadCountForTest = configReader.getNumberOfThreads();
        }

        // Лямбда для измерения времени
        auto measureTime = [&file, threadCountForTest](const std::function<void()>& func, const std::string& funcName, VectorT& vectorT) {
            static bool firstCall = true;
            if (!firstCall) { 
                file << '\n'; 
            } else { 
                firstCall = false; 
            }

            file << funcName;

            for(int i = 1; i <= threadCountForTest; ++i) {
                vectorT.setThreadCount(i);
                auto startTime = std::chrono::high_resolution_clock::now();

                func();  // Выполняем переданную функцию

                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = endTime - startTime;

                file << '\n' << elapsed.count();
            }
        };

        // Выполняем тесты
        measureTime([&]() { vectorT.minimumValue(); }, "minimumValue", vectorT);
        measureTime([&]() { vectorT.minimumIndexByValue(); }, "minimumIndexByValue", vectorT);
        measureTime([&]() { vectorT.maximumValue(); }, "maximumValue", vectorT);
        measureTime([&]() { vectorT.maximumIndexByValue(); }, "maximumIndexByValue", vectorT);
        measureTime([&]() { vectorT.sumValue(); }, "sumValue", vectorT);
        measureTime([&]() { vectorT.avgValue(); }, "avgValue", vectorT);
        measureTime([&]() { vectorT.euclidMonheton(); }, "euclidMonheton", vectorT);
        measureTime([&]() { vectorT.scalarMultiply(vectorT); }, "scalarMultiply", vectorT);
    

        file.close();
        std::cout << "Данные времени тестирования успешно созданы: " << fileName << '\n';
    }
};