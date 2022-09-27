#include "network.h"

Network::Network(int sd) {
    number_of_nodes = 0;
    snapshotDelay = sd;
}

void Network::add_nodes(std::vector<Node*> ns) {
    printf("Adding nodes to the network\n");
    nodes = ns;
    number_of_nodes = nodes.size();
}

void Network::add_neighbour(int id, std::vector<int> neighbours) {
    // Add here
    std::vector<Node> final_neighbours;
    for(int i=0; i<neighbours.size(); i++) {
        final_neighbours.push_back(*this->nodes[neighbours[i]]);
    }
    this->nodes[id]->neighbours = final_neighbours;
}

void Network::run() {
    printf("Running the network\n");
    this->nodes[0]->process_message();
}