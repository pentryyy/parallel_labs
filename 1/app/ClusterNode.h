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
    bool ImportSpec(std::ifstream& file, const std::string& specHeader, T& specT) {
        std::string header;
        std::getline(file, header);
        if (header != specHeader) {
            std::cerr << "Неверный формат файла. Ожидается заголовок '" << specHeader << "'." << std::endl;
            return false;
        }
        specT.Import(file);
        return true;
    }

    template <typename T>
    bool ExportSpec(std::ofstream& file, const std::string& specHeader, const T& specT) const {
        file << std::endl << specHeader << std::endl;
        specT.Export(file);
        return true;
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

    bool ImportNode(std::ifstream& in) {
        if (!ImportSpec(in, "GpuSpec", gpu) ||
            !ImportSpec(in, "CpuSpec", cpu) ||
            !ImportSpec(in, "RamSpec", ram) ||
            !ImportSpec(in, "LanSpec", lan)) {
            std::cerr << "Не загрузился один из компонентов." << std::endl;
            return false;
        }
        return true;
    }

    bool ExportNode(std::ofstream& out) const {
        if (!ExportSpec(out, "GpuSpec", gpu) ||
            !ExportSpec(out, "CpuSpec", cpu) ||
            !ExportSpec(out, "RamSpec", ram) ||
            !ExportSpec(out, "LanSpec", lan)) {
            std::cerr << "Не удалось экспортировать один из компонентов." << std::endl;
            return false;
        }
        return true;
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
        if (!ImportSpec(file, "GpuSpec", gpu) ||
            !ImportSpec(file, "CpuSpec", cpu) ||
            !ImportSpec(file, "RamSpec", ram) ||
            !ImportSpec(file, "LanSpec", lan)) {
            std::cerr << "Не загрузился один из компонентов." << std::endl;
            return false;
        }
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
        file << "ClusterNode";
        if (!ExportSpec(file, "GpuSpec", gpu) ||
            !ExportSpec(file, "CpuSpec", cpu) ||
            !ExportSpec(file, "RamSpec", ram) ||
            !ExportSpec(file, "LanSpec", lan)) {
            std::cerr << "Не удалось экспортировать один из компонентов." << std::endl;
            return false;
        }
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};