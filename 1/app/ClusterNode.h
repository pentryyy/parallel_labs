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

    template <typename T>
    bool ImportSpec(std::ifstream& file, T& specT) {   
        if (!specT.Import(file)) { return false; }
        return true; 
    }

    template <typename T>
    void ExportSpec(std::ofstream& file, const T& specT) const {
        file << std::endl;
        specT.Export(file);
    }
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
        std::cout << "Спецификации узла кластера:" << std::endl;
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    bool Import(std::ifstream& in) {
        if (!ImportSpec(in, gpu) ||
            !ImportSpec(in, cpu) ||
            !ImportSpec(in, ram) ||
            !ImportSpec(in, lan)) {
            std::cerr << "Не загрузился один из компонентов." << std::endl;
            return false;
        }
        return true;
    }

    void Export(std::ofstream& out) const {
        out << "ClusterNode";
        ExportSpec(out, gpu);
        ExportSpec(out, cpu);
        ExportSpec(out, ram);
        ExportSpec(out, lan);
    }

    bool Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        std::string header;
        std::getline(file, header);
        if (header != "ClusterNode") {
            std::cerr << "Неверный формат файла. Ожидается заголовок 'ClusterNode'." << std::endl;
            return false;
        }
        if (!Import(file)) { return false; }
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << fileName << std::endl;
        return true;
    }
    
    bool Export(const std::string& fileName) const {
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи: " << fileName << std::endl;
            return false;
        }
        Export(file);
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};