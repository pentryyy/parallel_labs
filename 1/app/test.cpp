#include "main/Cluster.h"
#include <iostream>
#include <sstream>
#include <cassert>

const std::string GREEN_COLOR = "\033[32m";
const std::string RED_COLOR = "\033[31m";
const std::string RESET_COLOR = "\033[0m";

/*---------------------------GPU SPEC TESTS------------------------------------*/

void TestGpuSpecConstructor() {
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    std::string expectedOutput = "GpuSpec";
    std::string actualOutput = gpu.GetClassHeader();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "GpuSpec Constructor passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "GpuSpec Constructor failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestGpuSpecExport() {
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    std::stringstream ss;
    gpu.Export(ss);
    std::string expectedOutput = "GpuSpec\nRTX 3080\n10240\n8704\n1440\n19000";
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "GpuSpec Export passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "GpuSpec Export failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestGpuSpecImport() {
    std::stringstream ss("GpuSpec\nRTX 3080\n10240\n8704\n1440\n19000");
    GpuSpec gpu;
    assert(gpu.Import(ss) == true);
    std::stringstream exportedData;
    gpu.Export(exportedData);
    std::string expectedOutput = "GpuSpec\nRTX 3080\n10240\n8704\n1440\n19000";
    std::string actualOutput = exportedData.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "GpuSpec Import passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "GpuSpec Import failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestGpuSpecPrint() { 
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    std::string expectedOutput = "Модель видеокарты: RTX 3080, Объем памяти: 10240 МБ, "
                                  "Количество ядер: 8704, Частота ядер: 1440 МГц, "
                                  "Частота VRAM: 19000 МГц\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    gpu.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "GpuSpec Print passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "GpuSpec Print failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput;
        std::cout << "Actual: " << actualOutput;
    }
}

/*---------------------------CPU SPEC TESTS------------------------------------*/

void TestCpuSpecConstructor() {
    CpuSpec cpu("Intel i9", 8, 16, true, true, 3200.0, 2400.0, 1000.0);
    std::string expectedOutput = "CpuSpec";
    std::string actualOutput = cpu.GetClassHeader();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "CpuSpec Constructor passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "CpuSpec Constructor failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestCpuSpecExport() {
    CpuSpec cpu("Intel i9", 8, 16, true, true, 3200.0, 2400.0, 1000.0);
    std::stringstream ss;
    cpu.Export(ss);
    std::string expectedOutput = "CpuSpec\nIntel i9\n8\n16\n1\n1\n3200\n2400\n1000";
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "CpuSpec Export passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "CpuSpec Export failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestCpuSpecImport() {
    std::stringstream ss("CpuSpec\nIntel i9\n8\n16\n1\n1\n3200\n2400\n1000");
    CpuSpec cpu;
    assert(cpu.Import(ss) == true);
    std::stringstream exportedData;
    cpu.Export(exportedData);
    std::string expectedOutput = "CpuSpec\nIntel i9\n8\n16\n1\n1\n3200\n2400\n1000";
    std::string actualOutput = exportedData.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "CpuSpec Import passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "CpuSpec Import failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestCpuSpecPrint() { 
    CpuSpec cpu("Intel i9", 8, 16, true, true, 3200.0, 2400.0, 1000.0);
    std::string expectedOutput = "Модель процессора: Intel i9, Количество производительных ядер: 8, "
                                  "Количество эффективных ядер: 16, Наличие E-core: Да, "
                                  "Наличие многопоточности: Да, Частота производительных ядер: 3200 МГц, "
                                  "Частота эффективных ядер: 2400.000000 МГц, Частота межъядерной шины: 1000 МГц\n";                                
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    cpu.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "CpuSpec Print passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "CpuSpec Print failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput;
        std::cout << "Actual: " << actualOutput;
    }
}

/*---------------------------RAM SPEC TESTS------------------------------------*/

void TestRamSpecConstructor() {
    RamSpec ram(16384, 3200.0, "DDR4");
    std::string expectedOutput = "RamSpec";
    std::string actualOutput = ram.GetClassHeader();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "RamSpec Constructor passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "RamSpec Constructor failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestRamSpecExport() {
    RamSpec ram(16384, 3200.0, "DDR4");
    std::stringstream ss;
    ram.Export(ss);
    std::string expectedOutput = "RamSpec\n16384\n3200\nDDR4";
    std::string actualOutput = ss.str();
    
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "RamSpec Export passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "RamSpec Export failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestRamSpecImport() {
    std::stringstream ss("RamSpec\n16384\n3200\nDDR4");
    RamSpec ram;
    assert(ram.Import(ss) == true);
    std::stringstream exportedData;
    ram.Export(exportedData);
    std::string expectedOutput = "RamSpec\n16384\n3200\nDDR4";
    std::string actualOutput = exportedData.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "RamSpec Import passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "RamSpec Import failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestRamSpecPrint() { 
    RamSpec ram(16384, 3200.0, "DDR4");
    std::string expectedOutput = "Объем ОЗУ: 16384 МБ, Частота: 3200 МГц, Тип: DDR4\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    ram.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "RamSpec Print passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "RamSpec Print failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput;
        std::cout << "Actual: " << actualOutput;
    }
}

/*---------------------------LAN SPEC TESTS------------------------------------*/

void TestLanSpecConstructor() {
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    std::string expectedOutput = "LanSpec";
    std::string actualOutput = lan.GetClassHeader();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "LanSpec Constructor passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "LanSpec Constructor failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestLanSpecExport() {
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    std::stringstream ss;
    lan.Export(ss);
    std::string expectedOutput = "LanSpec\n1000\n00:1A:2B:3C:4D:5E";
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "LanSpec Export passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "LanSpec Export failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestLanSpecImport() {
    std::stringstream ss("LanSpec\n1000\n00:1A:2B:3C:4D:5E");
    LanSpec lan;
    assert(lan.Import(ss) == true);
    std::stringstream exportedData;
    lan.Export(exportedData);
    std::string expectedOutput = "LanSpec\n1000\n00:1A:2B:3C:4D:5E";
    std::string actualOutput = exportedData.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "LanSpec Import passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "LanSpec Import failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestLanSpecPrint() { 
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    std::string expectedOutput = "Скорость LAN: 1000 Мбит/с, MAC-адрес: 00:1A:2B:3C:4D:5E\n";
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    lan.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "LanSpec Print passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "LanSpec Print failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput;
        std::cout << "Actual: " << actualOutput;
    }
}

/*-------------------------CLUSTER NODE TESTS-----------------------------------*/

void TestClusterNodeConstructor() {
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    CpuSpec cpu("Intel Core i7", 8, 0, true, true, 3200.0, 0.0, 0.0);
    RamSpec ram(16384, 3200.0, "DDR4");
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    ClusterNode node(gpu, cpu, ram, lan);
    std::string expectedOutput = "ClusterNode";
    std::string actualOutput = node.GetClassHeader();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "ClusterNode Constructor passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "ClusterNode Constructor failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestClusterNodeExport() {
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    CpuSpec cpu("Intel Core i7", 8, 0, true, true, 3200.0, 0.0, 0.0);
    RamSpec ram(16384, 3200.0, "DDR4");
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    ClusterNode node(gpu, cpu, ram, lan);
    std::stringstream ss;
    node.Export(ss);
    std::string expectedOutput = "ClusterNode\nGpuSpec\nRTX 3080\n10240\n8704\n1440\n19000\n"
                                  "CpuSpec\nIntel Core i7\n8\n0\n1\n1\n3200\n0\n0\n"
                                  "RamSpec\n16384\n3200\nDDR4\n"
                                  "LanSpec\n1000\n00:1A:2B:3C:4D:5E";

    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "ClusterNode Export passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "ClusterNode Export failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestClusterNodeImport() {
    std::stringstream ss("ClusterNode\n"
                         "GpuSpec\nRTX 3080\n10240\n8704\n1440\n19000\n"
                         "CpuSpec\nIntel Core i7\n8\n0\n1\n1\n3200\n0\n0\n"
                         "RamSpec\n16384\n3200\nDDR4\n"
                         "LanSpec\n1000\n00:1A:2B:3C:4D:5E");

    ClusterNode node;
    assert(node.Import(ss) == true);
    std::stringstream exportedData;
    node.Export(exportedData);
    std::string expectedOutput = "ClusterNode\nGpuSpec\nRTX 3080\n10240\n8704\n1440\n19000\n"
                                  "CpuSpec\nIntel Core i7\n8\n0\n1\n1\n3200\n0\n0\n"
                                  "RamSpec\n16384\n3200\nDDR4\n"
                                  "LanSpec\n1000\n00:1A:2B:3C:4D:5E";
    std::string actualOutput = exportedData.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "ClusterNode Import passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "ClusterNode Import failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput << std::endl;
        std::cout << "Actual: " << actualOutput << std::endl;
    }
}

void TestClusterNodePrint() {
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    CpuSpec cpu("Intel Core i7", 8, 0, true, true, 3200.0, 0.0, 0.0);
    RamSpec ram(16384, 3200.0, "DDR4");
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    ClusterNode node(gpu, cpu, ram, lan);
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    node.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    std::string expectedOutput =
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
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "ClusterNode Print passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "ClusterNode Print failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << expectedOutput;
        std::cout << "Actual: " << actualOutput;
    }
}

/*-----------------------------CLUSTER TESTS------------------------------------*/

void TestClusterPrint() {
    Cluster cluster;
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    CpuSpec cpu("Intel Core i7", 8, 0, true, true, 3200.0, 0.0, 0.0);
    RamSpec ram(16384, 3200.0, "DDR4");
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    ClusterNode node(gpu, cpu, ram, lan);
    cluster.AddNode(node);
    std::stringstream ss;
    auto oldCoutBuf = std::cout.rdbuf(ss.rdbuf());
    cluster.Print();
    std::cout.rdbuf(oldCoutBuf);
    std::string actualOutput = ss.str();
    std::string expectedOutput =
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
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "Cluster Print passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "Cluster Print failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected:\n" << expectedOutput << std::endl;
        std::cout << "Actual:\n" << actualOutput << std::endl;
    }
}

void TestClusterImport() {
    std::stringstream ss(
        "Cluster\n"
        "ClusterNode\n"
        "GpuSpec\n"
        "RTX 3080\n"
        "10240\n"
        "8704\n"
        "1440\n"
        "19000\n"
        "CpuSpec\n"
        "Intel Core i7\n"
        "8\n"
        "0\n"
        "1\n"
        "1\n"
        "3200\n"
        "0\n"
        "0\n"
        "RamSpec\n"
        "16384\n"
        "3200\n"
        "DDR4\n"
        "LanSpec\n"
        "1000\n"
        "00:1A:2B:3C:4D:5E"
    );
    Cluster cluster;
    cluster.Import(ss);
    std::stringstream exportStream;
    cluster.Export(exportStream);
    std::string actualOutput = exportStream.str();
    std::string expectedOutput =
        "Cluster\n"
        "ClusterNode\n"
        "GpuSpec\n"
        "RTX 3080\n"
        "10240\n"
        "8704\n"
        "1440\n"
        "19000\n"
        "CpuSpec\n"
        "Intel Core i7\n"
        "8\n"
        "0\n"
        "1\n"
        "1\n"
        "3200\n"
        "0\n"
        "0\n"
        "RamSpec\n"
        "16384\n"
        "3200\n"
        "DDR4\n"
        "LanSpec\n"
        "1000\n"
        "00:1A:2B:3C:4D:5E";
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "Cluster Import passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "Cluster Import failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << std::endl << expectedOutput << std::endl;
        std::cout << "Actual: " << std::endl << actualOutput << std::endl;
    }
}

void TestClusterExport() {
    GpuSpec gpu("RTX 3080", 10240, 8704, 1440.0, 19000.0);
    CpuSpec cpu("Intel Core i7", 8, 0, true, true, 3200.0, 0.0, 0.0);
    RamSpec ram(16384, 3200.0, "DDR4");
    LanSpec lan(1000.0, "00:1A:2B:3C:4D:5E");
    ClusterNode node(gpu, cpu, ram, lan);
    Cluster cluster;
    cluster.AddNode(node);
    std::stringstream ss;
    cluster.Export(ss);
    std::string expectedOutput =
        "Cluster\n"
        "ClusterNode\n"
        "GpuSpec\n"
        "RTX 3080\n"
        "10240\n"
        "8704\n"
        "1440\n"
        "19000\n"
        "CpuSpec\n"
        "Intel Core i7\n"
        "8\n"
        "0\n"
        "1\n"
        "1\n"
        "3200\n"
        "0\n"
        "0\n"
        "RamSpec\n"
        "16384\n"
        "3200\n"
        "DDR4\n"
        "LanSpec\n"
        "1000\n"
        "00:1A:2B:3C:4D:5E";
    std::string actualOutput = ss.str();
    if (actualOutput == expectedOutput) {
        std::cout << GREEN_COLOR << "Cluster Export passed!" << RESET_COLOR << std::endl;
    } else {
        std::cout << RED_COLOR << "Cluster Export failed!" << RESET_COLOR << std::endl;
        std::cout << "Expected: " << std::endl << expectedOutput << std::endl;
        std::cout << "Actual: " << std::endl << actualOutput << std::endl;
    }
}

int main() {
    // GpuSpec tests
    TestGpuSpecConstructor();
    TestGpuSpecPrint();
    TestGpuSpecExport();
    TestGpuSpecImport();

    // CpuSpec tests
    TestCpuSpecConstructor();
    TestCpuSpecPrint();
    TestCpuSpecExport();
    TestCpuSpecImport();
   
    // RamSpec tests
    TestRamSpecConstructor();
    TestRamSpecExport();
    TestRamSpecImport();
    TestRamSpecPrint();

    // LanSpec tests
    TestLanSpecConstructor();
    TestLanSpecExport();
    TestLanSpecImport();
    TestLanSpecPrint();

    // ClusterNode tests
    TestClusterNodeConstructor();
    TestClusterNodeExport();
    TestClusterNodeImport();
    TestClusterNodePrint();

    // Cluster tests
    TestClusterExport();
    TestClusterImport();
    TestClusterPrint();
    return 0;
}