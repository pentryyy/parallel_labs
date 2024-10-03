#include <vector>
#include <fstream>
#include "ClusterNode.h"

class Cluster  {
private:
    std::vector<ClusterNode> nodes;
public:
    void AddNode(const ClusterNode &node) {
        nodes.push_back(node);
    }

    void Print() const {
        std::cout << "Cluster contains " << nodes.size() << " nodes:\n";
        for (const auto &node : nodes) {
            node.Print();
            std::cout << std::string(128, '-') << std::endl;
        }
    }

    void Import(const std::string &filename) {
        std::ifstream in(filename);
        if (in.is_open()) {
            int nodeCount;
            in >> nodeCount;
            nodes.resize(nodeCount);
            for (auto &node : nodes) {
                node.Import(in);
            }
            in.close();
        }
    }

    void Export(const std::string &filename) const {
        std::ofstream out(filename);
        if (out.is_open()) {
            out << nodes.size() << std::endl;
            for (const auto &node : nodes) {
                node.Export(out);
            }
            out.close();
        }
    }
};