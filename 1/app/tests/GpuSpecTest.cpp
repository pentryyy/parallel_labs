#include <gtest/gtest.h>
#include "../src/GpuSpec.h"
#include <sstream>

namespace GpuData { 
    std::string expectedOutput = "GpuSpec\nRTX 3080\n10240\n8704\n1440\n19000";
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
}

TEST(GpuSpecTest, Constructor) {
    EXPECT_NE(GpuData::gpu.GetModel(), "") << "Поле model пусто!";
    EXPECT_GT(GpuData::gpu.GetMemorySize(), 0) << "Поле memorySize равно 0!";
    EXPECT_GT(GpuData::gpu.GetCoreCount(), 0) << "Поле coreCount равно 0!";
    EXPECT_GT(GpuData::gpu.GetCoreFrequency(), 0.0) << "Поле coreFrequency равно 0!";
    EXPECT_GT(GpuData::gpu.GetVramFrequency(), 0.0) << "Поле vramFrequency равно 0!";
}

TEST(GpuSpecTest, Export) {
    std::stringstream ss;
    GpuData::gpu.Export(ss);
    EXPECT_EQ(ss.str(), GpuData::expectedOutput) << "Экспорт не удался!";
}

TEST(GpuSpecTest, Import) {
    std::stringstream ss(GpuData::expectedOutput);
    GpuSpec gpu;
    ASSERT_TRUE(gpu.Import(ss)) << "Импорт вернул значение false!";
    std::stringstream exportedData;
    gpu.Export(exportedData);
    EXPECT_EQ(exportedData.str(), GpuData::expectedOutput) << "Импорт не выполнен!";
}

TEST(GpuSpecTest, Print) {
    std::string expectedOutput = "Модель видеокарты: RTX 3080, Объем памяти: 10240 МБ, "
                                  "Количество ядер: 8704, Частота ядер: 1440 МГц, "
                                  "Частота VRAM: 19000 МГц\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    GpuData::gpu.Print();
    std::cout.rdbuf(oldCoutBuf);
    EXPECT_EQ(ss.str(), expectedOutput) << "Ошибка печати!";
}