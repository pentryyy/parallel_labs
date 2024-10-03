#include "Cluster.h"

int main() {
    GpuSpec gpu1("NVIDIA RTX 3080", 10240, 8704, 1440.0, 1188.0);
    GpuSpec gpu2("AMD RX 6800 XT", 16384, 4608, 2250.0, 2000.0);
    CpuSpec cpu1("Intel Core i9-13900K", 8, 16, true, true, 5300.0, 4000.0);
    CpuSpec cpu2("AMD Ryzen 7 7800X3D", 8, 0, false, true, 4500.0, 0.0, 2133.0);
    RamSpec ram1(49152, 7200.0, "DDR5");
    RamSpec ram2(32768, 6400.0, "DDR5");
    LanSpec lan1(1000.0, "00:1A:2B:3C:4D:5E");
    LanSpec lan2(2500.0, "00:1B:2C:3D:4E:5F");

    ClusterNode pc1(gpu1, cpu1, ram1, lan1);
    ClusterNode pc2(gpu2, cpu2, ram2, lan2);

    Cluster myCluster;

    myCluster.AddNode(pc1);
    myCluster.AddNode(pc2);

    myCluster.Print();
    return 0;
}