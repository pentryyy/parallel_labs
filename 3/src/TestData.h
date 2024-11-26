#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>

class TestData {
private:
    std::string      funcName;
    std::list<float> funcTimeValues;

public:
    TestData(const std::string& funcName, const std::list<float>& funcTimeValues) :
        funcName(funcName),
        funcTimeValues(funcTimeValues) {}

    std::string getFuncName() const {
        return funcName;
    }

    float findMin() const {
        return *std::min_element(funcTimeValues.begin(), funcTimeValues.end());
    }

    float findMax() const {
        return *std::max_element(funcTimeValues.begin(), funcTimeValues.end());
    }

    float calculateMean() const {
        float sum = std::accumulate(funcTimeValues.begin(), funcTimeValues.end(), 0.0f);
        return sum / funcTimeValues.size();
    }

    float calculateMedian() const {
        std::vector<float> sortedValues(funcTimeValues.begin(), funcTimeValues.end());
        std::sort(sortedValues.begin(), sortedValues.end());

        size_t size = sortedValues.size();
        if (size % 2 == 0) {
            return (sortedValues[size / 2 - 1] + sortedValues[size / 2]) / 2.0f;
        } else {
            return sortedValues[size / 2];
        }
    }

    float calculatePercentile95() const {
        std::vector<float> sortedValues(funcTimeValues.begin(), funcTimeValues.end());
        std::sort(sortedValues.begin(), sortedValues.end());

        size_t index = static_cast<size_t>(std::ceil(0.95 * sortedValues.size())) - 1;
        return sortedValues[index];
    }
};