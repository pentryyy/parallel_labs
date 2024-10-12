#include <gtest/gtest.h>
#include "../src/LanSpec.h"
#include <sstream>

namespace LanData {
    std::string expectedOutput = "LanSpec\n1000\n00:1A:2B:3C:4D:5E";
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
}

TEST(LanSpecTest, Constructor) {
    EXPECT_GT(LanData::lan.GetSpeed(), 0.0) << "Поле speed равно 0!";
    EXPECT_NE(LanData::lan.GetMacAddress(), "") << "Поле macAddress пусто!";
}

TEST(LanSpecTest, Export) {
    std::stringstream ss;
    LanData::lan.Export(ss);
    EXPECT_EQ(ss.str(), LanData::expectedOutput) << "Экспорт не удался!";
}

TEST(LanSpecTest, Import) {
    std::stringstream ss(LanData::expectedOutput);
    LanSpec lan;
    ASSERT_TRUE(lan.Import(ss)) << "Импорт вернул значение false!";
    std::stringstream exportedData;
    lan.Export(exportedData);
    EXPECT_EQ(exportedData.str(), LanData::expectedOutput) << "Импорт не выполнен!";
}

TEST(LanSpecTest, Print) {
    std::string expectedOutput = "Скорость LAN: 1000 Мбит/с, MAC-адрес: 00:1A:2B:3C:4D:5E\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    LanData::lan.Print();
    std::cout.rdbuf(oldCoutBuf);
    EXPECT_EQ(ss.str(), expectedOutput) << "Ошибка печати!";
}