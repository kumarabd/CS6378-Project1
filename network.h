#include<iostream>
#include <vector>
#include "node.h"

class Network {
    private:
        int number_of_nodes;
        int snapshotDelay;
        std::vector<Node*> nodes;
    public:
        Network(int sd);
        void add_nodes(std::vector<Node*> n);
        void add_neighbour(int id, std::vector<int> neighbours);
        void run();
};