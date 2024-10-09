#include <gtest/gtest.h>
#include "../src/Cluster.h"
#include <sstream>

namespace ClusterData {
    std::string clusterNodeExpectedOutput = 
        "ClusterNode\nGpuSpec\nRTX 3080\n10240\n8704\n1440\n19000\n"
        "CpuSpec\nIntel Core i7\n8\n0\n1\n1\n3200\n0\n0\n"
        "RamSpec\n16384\n3200\nDDR4\n"
        "LanSpec\n1000\n00:1A:2B:3C:4D:5E";
    std::string clusterExpectedOutput = "Cluster\n" + clusterNodeExpectedOutput;
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    CpuSpec cpu("Intel Core i7", 8, 0, true, true, 3200.0, 0.0, 0.0);
    RamSpec ram(16384, 3200.0, "DDR4");
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    ClusterNode node(gpu, cpu, ram, lan);
}

TEST(ClusterNodeTest, Constructor) {
    EXPECT_EQ(ClusterData::node.GetClassHeader(), "ClusterNode") << "Ошибка конструктора!";
}

TEST(ClusterNodeTest, Export) {
    std::stringstream ss;
    ClusterData::node.Export(ss);
    EXPECT_EQ(ss.str(), ClusterData::clusterNodeExpectedOutput) << "Экспорт не удался!";
}

TEST(ClusterNodeTest, Import) {
    std::stringstream ss(ClusterData::clusterNodeExpectedOutput);
    ClusterNode node;
    ASSERT_TRUE(node.Import(ss)) << "Импорт вернул значение false!";
    std::stringstream exportedData;
    node.Export(exportedData);
    EXPECT_EQ(exportedData.str(), ClusterData::clusterNodeExpectedOutput) << "Импорт не выполнен корректно!";
}

TEST(ClusterNodeTest, Print) {
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    ClusterData::node.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string clusterNodeExpectedOutput =
        "Спецификации узла кластера:\n"
        "Модель видеокарты: RTX 3080, Объем памяти: 10240 МБ, "
        "Количество ядер: 8704, Частота ядер: 1440 МГц, "
        "Частота VRAM: 19000 МГц\n"
        "Модель процессора: Intel Core i7, Количество производительных ядер: 8, "
        "Количество эффективных ядер: Нет, Наличие E-core: Да, "
        "Наличие многопоточности: Да, Частота производительных ядер: 3200 МГц, "
        "Частота эффективных ядер: 0.000000 МГц, Частота межъядерной шины: 0 МГц\n"
        "Объем ОЗУ: 16384 МБ, Частота: 3200 МГц, Тип: DDR4\n"
        "Скорость LAN: 1000 Мбит/с, MAC-адрес: 00:1A:2B:3C:4D:5E\n";
    EXPECT_EQ(ss.str(), clusterNodeExpectedOutput) << "Ошибка печати!";
}

TEST(ClusterTest, Import) {
    Cluster cluster;
    std::stringstream ss(ClusterData::clusterExpectedOutput);
    ASSERT_TRUE(cluster.Import(ss)) << "Импорт вернул значение false!";
    std::stringstream exportedData;
    cluster.Export(exportedData);
    EXPECT_EQ(exportedData.str(), ClusterData::clusterExpectedOutput) << "Импорт не выполнен корректно!";
}

TEST(ClusterTest, Export) {
    Cluster cluster;
    cluster.AddNode(ClusterData::node);
    std::stringstream ss;
    cluster.Export(ss);
    EXPECT_EQ(ss.str(), ClusterData::clusterExpectedOutput) << "Экспорт не удался!";
}

TEST(ClusterTest, Print) {
    Cluster cluster;
    cluster.AddNode(ClusterData::node);
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    cluster.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    std::string clusterExpectedOutput =
        "Кластер содержит 1 узлов:\n"
        "------------------------------------------------------------------------------------------------\n"
        "Спецификации узла кластера:\n"
        "Модель видеокарты: RTX 3080, Объем памяти: 10240 МБ, "
        "Количество ядер: 8704, Частота ядер: 1440 МГц, "
        "Частота VRAM: 19000 МГц\n"
        "Модель процессора: Intel Core i7, Количество производительных ядер: 8, "
        "Количество эффективных ядер: Нет, Наличие E-core: Да, "
        "Наличие многопоточности: Да, Частота производительных ядер: 3200 МГц, "
        "Частота эффективных ядер: 0.000000 МГц, Частота межъядерной шины: 0 МГц\n"
        "Объем ОЗУ: 16384 МБ, Частота: 3200 МГц, Тип: DDR4\n"
        "Скорость LAN: 1000 Мбит/с, MAC-адрес: 00:1A:2B:3C:4D:5E\n"
        "------------------------------------------------------------------------------------------------\n";
    EXPECT_EQ(ss.str(), clusterExpectedOutput) << "Ошибка печати!";
}