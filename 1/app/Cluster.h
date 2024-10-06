#include <vector>
#include <fstream>
#include "ClusterNode.h"

class Cluster : public InputOutput{
private:
    std::vector<ClusterNode> nodes;
public:
    void AddNode(const ClusterNode &node) { nodes.push_back(node); }

    ~Cluster() {}

    std::string GetClassHeader() const override { return "Cluster"; }

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

    bool Import(std::istream& in) override {
        if (!CheckHeaderName(in)) { return false; }
        while (in) {
            ClusterNode node;
            if (!node.Import(in)) { return false; }
            nodes.push_back(node);
        }
        return true;
    }

    void Export(std::ostream& out) const override {
        out << GetClassHeader();
        for (const auto& node : nodes) {
            out << std::endl;
            node.Export(out);
        }
    }
};