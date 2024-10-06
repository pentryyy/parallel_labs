#include "GpuSpec.h"
#include "CpuSpec.h"
#include "RamSpec.h"
#include "LanSpec.h"

class ClusterNode : public InputOutput  {
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

    std::string GetClassHeader() const override { return "ClusterNode"; }

    void Print() const {
        std::cout << "Спецификации узла кластера:" << std::endl;
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    bool Import(std::istream& in) override {
        if (!CheckHeaderName(in)) { return false; }
        const std::vector<InputOutput*> components = { &gpu, &cpu, &ram, &lan };
        for (const auto& component : components) {
            if (!component->Import(in)) {
                std::cerr << "Не загрузился один из компонентов." << std::endl;
                return false;
            }
        }
        return true;
    }

    void Export(std::ostream& out) const override {
        out << GetClassHeader();
        const std::vector<const InputOutput*> components = { &gpu, &cpu, &ram, &lan };
        for (const auto& component : components) {
            out << std::endl;
            component->Export(out);
        }
    }
};