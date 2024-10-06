#include "Cluster.h"
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <conio.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);

    GpuSpec gpu1("AMD RX 6800 XT", 16384, 4608, 2250.0, 2000.0);
    CpuSpec cpu1("AMD Ryzen 7 7800X3D", 8, 0, false, true, 4500.0, 0.0, 2133.0);
    RamSpec ram1(32768, 6400.0, "DDR5");
    LanSpec lan1(2500.0, "00:1B:2C:3D:4E:5F");

    GpuSpec gpuF;
    gpuF.ImportFromFile("gpu_import.txt");
    gpuF.ExportToFile("gpu_export.txt");

    CpuSpec cpuF;
    cpuF.ImportFromFile("cpu_import.txt");
    cpuF.ExportToFile("cpu_export.txt");

    RamSpec ramF;
    ramF.ImportFromFile("ram_import.txt");
    ramF.ExportToFile("ram_export.txt");

    LanSpec lanF;
    lanF.ImportFromFile("lan_import.txt");
    lanF.ExportToFile("lan_export.txt");

    ClusterNode clusterNode1(gpu1, cpu1, ram1, lan1);
    ClusterNode clusterNode2(gpuF, cpuF, ramF, lanF);
    
    ClusterNode clusterNodeF;
    clusterNodeF.Import("cluster_node_import.txt");
    clusterNodeF.Export("cluster_node_export.txt");
    
    Cluster сluster1;
    сluster1.AddNode(clusterNode1);
    сluster1.AddNode(clusterNode2);
    сluster1.AddNode(clusterNodeF);
    сluster1.Print();

    Cluster сlusterF;
    сlusterF.Import("cluster_import.txt");
    сlusterF.Export("cluster_export.txt");
    сlusterF.Print();

    std::cout << "Нажмите любую клавишу для продолжения... ";

    /* 
    Запись строк в память вместо стандартного вывода (консоли) 
    для корректного отображения кириллицы при запуске main.exe.
    */
    std::ostringstream buffer;
    std::streambuf* originalCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    std::cout.rdbuf(originalCoutBuffer);
    std::string output = buffer.str();

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideOutput = converter.from_bytes(output);

    getch();
    return 0;
}