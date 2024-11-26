#include <thread>
#include <chrono>              // Для замера времени
#include <functional>          // Для приема функции в качестве аргумента
#include "ConfigReader.h"
#include "HasSetThreadCount.h" // Структура для проверки наличия метода в классе
#include "TestData.h"          // Хранит данные о времени тестов

template <typename VectorT>
class PerformanceTest {
private:
    VectorT             vectorT = VectorT(1); // Шаблонный вектор, размерностью 1 элеменнт по умолчанию 
    ConfigReader        configReader;
    std::list<TestData> listOfTestData;       // Список результатов теста времени всех функций
public:
    PerformanceTest(VectorT& vectorT, const ConfigReader& configReader) :
        configReader(configReader) {
        this->vectorT.resize(configReader.getVectorSize()); // Изменяем размер вектора на тот, что в конфиге
        this->vectorT = vectorT;                            // Присваиваем вектор, переданный в конструктор
    }

    void runTimeTest() {
        int threadCountForTest; // Будет хранить количество потоков на основе конфига
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
            std::list<float> funcTimeValues; // Для хранения времени выполнения тестов
            
            // Для того, чтобы сделать переход строки перед названием функции
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
                    func();
                    auto endTime = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<double> elapsed = endTime - startTime;
                    threadSum += elapsed.count();
                }

                float avgForThread = threadSum / configReader.getNumberOfTests(); // Вычисляем среднее для текущего потока
                file << '\n' << avgForThread;
                funcTimeValues.push_back(avgForThread);
            }
            TestData testData(funcName, funcTimeValues); // Результаты текущей функции
            listOfTestData.push_back(testData);          // Добавляем в список результаты текущей функции
        };

        VectorT vectorTInverted(configReader.getVectorSize());
        vectorTInverted = vectorT;
        vectorTInverted.invertValues();

        // Выполняем тесты времени
        measureTime([this]() { vectorT.minimumValue(); }, "minimumValue");
        measureTime([this]() { vectorT.minimumIndexByValue(); }, "minimumIndexByValue");
        measureTime([this]() { vectorT.maximumValue(); }, "maximumValue");
        measureTime([this]() { vectorT.maximumIndexByValue(); }, "maximumIndexByValue");
        measureTime([this]() { vectorT.sumValue(); }, "sumValue");
        measureTime([this]() { vectorT.avgValue(); }, "avgValue"); 
        measureTime([this]() { vectorT.euclidMonheton(); }, "euclidMonheton");
        measureTime([this, &vectorTInverted]() { vectorT.scalarMultiply(vectorTInverted); }, "scalarMultiply");
    
        file.close();
        std::cout << "Данные времени тестирования успешно созданы: " << fileName << '\n';
    }

    void runDataTest() {
        /*
        Своеобразная проверка на то, 
        что явялется ли данный вектор многопоточным
        */
        if constexpr (HasSetThreadCount<VectorT>::value) {
            vectorT.setThreadCount(1);
        } else {
            throw std::runtime_error("Данный вектор однопоточный");
        }

        if (listOfTestData.size() == 0) {
            throw std::runtime_error("Невозможно расчитать метрики, так как нет данных о времени выполнения функций");
        }

        std::string fileName = configReader.getfileForTestMetrics();
        
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для записи: " + fileName);
        }

        for (const auto& itemOfTestData : listOfTestData) {
            // Для того, чтобы сделать переход строки перед названием функции
            static bool firstCall = true;   

            if (!firstCall) { 
                file << '\n'; 
            } else { 
                firstCall = false; 
            }

            file << itemOfTestData.getFuncName();

            file << '\n' << "Минимальное значение:::"  << itemOfTestData.findMin();
            file << '\n' << "Максимальное значение:::" << itemOfTestData.findMax();
            file << '\n' << "Среднее значение:::"      << itemOfTestData.calculateMean();
            file << '\n' << "Среднее значение:::"      << itemOfTestData.calculateMedian();
            file << '\n' << "95-й процентиль:::"       << itemOfTestData.calculatePercentile95();
        }

        file.close();
        std::cout << "Данные метрик тестирования успешно созданы: " << fileName << '\n';
    }
};