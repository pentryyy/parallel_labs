#include "GpuSpec.h"
#include "CpuSpec.h"
#include "RamSpec.h"
#include "LanSpec.h"

class ClusterNode {
private:
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;
public:
    ClusterNode(const GpuSpec &gpu = {}, 
                const CpuSpec &cpu = {}, 
                const RamSpec &ram = {}, 
                const LanSpec &lan = {}
        ) : 
        gpu(gpu), 
        cpu(cpu), 
        ram(ram), 
        lan(lan)
    {}

    ~ClusterNode() {}

    void Print() const {
        std::cout << "Cluster Node Specifications:\n";
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Import(std::istream &in) {
        gpu.Import(in);
        cpu.Import(in);
        ram.Import(in);
        lan.Import(in);
    }

    void Export(std::ostream &out) const {
        gpu.Export(out);
        cpu.Export(out);
        ram.Export(out);
        lan.Export(out);
    }
};