#include <gtest/gtest.h>
#include "../src/CpuSpec.h"
#include <sstream>

namespace CpuData { 
    std::string expectedOutput = "CpuSpec\nIntel i9\n8\n16\n1\n1\n3200\n2400\n1000";
    CpuSpec cpu("Intel i9", 8, 16, true, true, 3200.0, 2400.0, 1000.0);
}

TEST(CpuSpecTest, Constructor) {
    EXPECT_NE(CpuData::cpu.GetModel(), "") << "Поле model пусто!";
    EXPECT_GT(CpuData::cpu.GetPcoreCount(), 0) << "Поле pcoreCount равно 0!";
    if (CpuData::cpu.HasEcore()){
        EXPECT_TRUE(CpuData::cpu.HasEcore()) << "Поле hasEcore не установлено!";
        EXPECT_GT(CpuData::cpu.GetEcoreCount(), 0) << "Поле ecoreCount равно 0!";
        EXPECT_GT(CpuData::cpu.GetEcoreFrequency(), 0.0) << "Поле ecoreFrequency равно 0!";

    }
    if (CpuData::cpu.HasMultithreading()){
        EXPECT_TRUE(CpuData::cpu.HasMultithreading()) << "Поле hasMultithreading не установлено!";
    }
    EXPECT_GT(CpuData::cpu.GetPcoreFrequency(), 0.0) << "Поле pcoreFrequency равно 0!";
    EXPECT_GT(CpuData::cpu.GetUncoreFrequency(), 0.0) << "Поле uncoreFrequency равно 0!";
}

TEST(CpuSpecTest, Export) {
    std::stringstream ss;
    CpuData::cpu.Export(ss);
    EXPECT_EQ(ss.str(), CpuData::expectedOutput) << "Экспорт не удался!";
}

TEST(CpuSpecTest, Import) {
    std::stringstream ss(CpuData::expectedOutput);
    CpuSpec cpu;
    ASSERT_TRUE(cpu.Import(ss)) << "Импорт вернул значение false!";
    std::stringstream exportedData;
    cpu.Export(exportedData);
    EXPECT_EQ(exportedData.str(), CpuData::expectedOutput) << "Импорт не выполнен!";
}

TEST(CpuSpecTest, Print) {
    std::string expectedOutput = "Модель процессора: Intel i9, Количество производительных ядер: 8, "
                                 "Количество эффективных ядер: 16, Наличие E-core: Да, "
                                 "Наличие многопоточности: Да, Частота производительных ядер: 3200 МГц, "
                                 "Частота эффективных ядер: 2400.000000 МГц, Частота межъядерной шины: 1000 МГц\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    CpuData::cpu.Print();
    std::cout.rdbuf(oldCoutBuf);
    EXPECT_EQ(ss.str(), expectedOutput) << "Ошибка печати!";
}