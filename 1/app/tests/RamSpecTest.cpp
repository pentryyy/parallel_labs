#include <gtest/gtest.h>
#include "../src/RamSpec.h"
#include <sstream>

namespace RamData {
    std::string expectedOutput = "RamSpec\n16384\n3200\nDDR4";
    RamSpec ram(16384, 3200.0, "DDR4");
}

TEST(RamSpecTest, Constructor) {
    EXPECT_GT(RamData::ram.GetCapacity(), 0) << "Поле capacity равно 0!";
    EXPECT_GT(RamData::ram.GetFrequency(), 0.0) << "Поле frequency равно 0!";
    EXPECT_NE(RamData::ram.GetType(), "") << "Поле type пусто!";
}

TEST(RamSpecTest, Export) {
    std::stringstream ss;
    RamData::ram.Export(ss);
    EXPECT_EQ(ss.str(), RamData::expectedOutput) << "Экспорт не удался!";
}

TEST(RamSpecTest, Import) {
    std::stringstream ss(RamData::expectedOutput);
    RamSpec ram;
    ASSERT_TRUE(ram.Import(ss)) << "Импорт вернул значение false!";
    std::stringstream exportedData;
    ram.Export(exportedData);
    EXPECT_EQ(exportedData.str(), RamData::expectedOutput) << "Импорт не выполнен!";
}

TEST(RamSpecTest, Print) {
    std::string expectedOutput = "Объем ОЗУ: 16384 МБ, Частота: 3200 МГц, Тип: DDR4\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    RamData::ram.Print();
    std::cout.rdbuf(oldCoutBuf);
    EXPECT_EQ(ss.str(), expectedOutput) << "Ошибка печати!";
}