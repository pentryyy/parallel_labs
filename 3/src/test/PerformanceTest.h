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
            // Для хранения времени выполнения тестов на каждый поток
            std::list<std::list<float>> allFuncTimeValues;
            
            // Для того, чтобы сделать переход строки перед названием функции
            static bool firstCall = true;   

            if (!firstCall) { 
                file << '\n'; 
            } else { 
                firstCall = false; 
            }

            file << funcName;

            for (int i = 1; i <= threadCountForTest; ++i) {
                float            threadSum = 0;  // Сумма времени для текущего потока
                std::list<float> funcTimeValues; // Для хранения времени повторов теста
                
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

                    std::chrono::duration<float> elapsed = endTime - startTime;
                    threadSum += elapsed.count();
                    funcTimeValues.push_back(elapsed.count());
                }

                file << '\n' << threadSum / configReader.getNumberOfTests();
                allFuncTimeValues.push_back(funcTimeValues);
            }
            TestData testData(funcName, allFuncTimeValues); // Результаты текущей функции
            listOfTestData.push_back(testData);             // Добавляем в список результаты текущей функции
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

            // Вывод метрик для каждого количества потоков
            for (size_t i = 1; i <= itemOfTestData.getSizeOfList(); ++i) {
                file << '\n' << i;

                file << '\n' << "Минимальное значение:::"  << itemOfTestData.findMin(i);
                file << '\n' << "Максимальное значение:::" << itemOfTestData.findMax(i);
                file << '\n' << "Среднее значение:::"      << itemOfTestData.calculateMean(i);
                file << '\n' << "Медианное значение:::"    << itemOfTestData.calculateMedian(i);
                file << '\n' << "95-й процентиль:::"       << itemOfTestData.calculatePercentile95(i);
            }
        }

        file.close();
        std::cout << "Данные метрик тестирования успешно созданы: " << fileName << '\n';
    }
};