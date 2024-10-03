#include "GpuSpec.h"
#include "CpuSpec.h"

int main() {
    GpuSpec rtx_3080("NVIDIA RTX 3080", 10240, 8704, 1440.0, 1188.0);
    GpuSpec rx_6800XT("AMD RX 6800 XT", 16384, 4608, 2250.0, 2000.0);
    CpuSpec i9_13900K("Intel Core i9-13900K", 8, 16, true, true, 5300.0, 4000.0, 4500.0);
    CpuSpec ryzen_7800X3D("AMD Ryzen 7 7800X3D", 8, 0, false, true, 4500.0, 0.0, 2133.0);
    rtx_3080.Print();
    rx_6800XT.Print();
    i9_13900K.Print();
    ryzen_7800X3D.Print();
    return 0;
}