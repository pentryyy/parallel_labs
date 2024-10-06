#include <vector>
#include <fstream>
#include "ClusterNode.h"

class Cluster {
private:
    std::vector<ClusterNode> nodes;
public:
    void AddNode(const ClusterNode &node) {
        nodes.push_back(node);
    }

    void Print() const {
        if (nodes.size()){
            std::cout << "Кластер содержит " << nodes.size() << " узлов:" << std::endl;
            std::cout << std::string(96, '-') << std::endl;
        } else {
            std::cout << std::string(96, '-') << std::endl;
        }
        for (const auto &node : nodes) {
            node.Print();
            std::cout << std::string(96, '-') << std::endl;
        }
    }

    bool Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        std::string header;
        std::getline(file, header);
        if (header != "Cluster") {
            std::cerr << "Неверный формат файла. Ожидается заголовок 'Cluster'." << std::endl;
            return false;
        }
        while (file) {
            std::getline(file, header);
            if (header == "ClusterNode") {
                ClusterNode node;
                if(!node.Import(file)) { return false; }
                nodes.push_back(node);
            } else if (header.empty()) { break; }
        }
        file.close();
        std::cout << "Импорт кластера из файла успешно выполнен: " << fileName << std::endl;
        return true;
    }
    
    bool Export(const std::string& fileName) const {
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи: " << fileName << std::endl;
            return false;
        }
        file << "Cluster";
        for (const auto& node : nodes) {
            file << std::endl;
            node.Export(file);
        }
        file.close();
        std::cout << "Экспорт кластера в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};