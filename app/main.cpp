#include "Cluster.h"

int main() {
    GpuSpec gpu1("AMD RX 6800 XT", 16384, 4608, 2250.0, 2000.0);
    CpuSpec cpu1("AMD Ryzen 7 7800X3D", 8, 0, false, true, 4500.0, 0.0, 2133.0);
    RamSpec ram1(32768, 6400.0, "DDR5");
    LanSpec lan1(2500.0, "00:1B:2C:3D:4E:5F");

    GpuSpec gpuF;
    gpuF.Import("gpu_import.txt");
    gpuF.Export("gpu_export.txt");
   
    CpuSpec cpuF;
    cpuF.Import("cpu_import.txt");
    cpuF.Export("cpu_export.txt");
   
    RamSpec ramF;
    ramF.Import("ram_import.txt");
    ramF.Export("ram_export.txt");
   
    LanSpec lanF;
    lanF.Import("lan_import.txt");
    lanF.Export("lan_export.txt");

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

    return 0;
}