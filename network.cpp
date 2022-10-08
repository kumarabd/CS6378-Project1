#include "network.h"

Network::Network(int sd) {
    this->number_of_nodes = 0;
    this->snapshotDelay = sd;
}

void Network::add_nodes(std::vector<Node> ns) {
    this->nodes = ns;
    this->number_of_nodes = this->nodes.size();
    this->message_counter = this->number_of_nodes;
}

void Network::add_neighbour(int id, std::vector<int> neighbours) {
    for(int i=0; i<neighbours.size(); i++) {
        this->nodes[id].neighbours.push_back(&this->nodes[neighbours[i]]);
    }
}

void Network::run() {
    printf("Running the network\n");
<<<<<<< HEAD
    //for(int i=0; i<this->nodes.size();i++) {
    //    this->nodes[i]->info();
    //}
    //this->nodes[0]->info();
    this->nodes[0].process_message();
=======
    // this->nodes[0]->process_message(1);
    this->nodes[0]->process_message(0,"start");
>>>>>>> cb2f0e63d4086ea9dee8a242ea9587f39b749752
    while(this->message_counter) {
        this->message_counter--;
    }
    //if(this->verify_consistency()) {
    //    printf("State consistent\n");
    //}else {
    //    printf("State inconsistent\n");
    //}
}

bool Network::verify_consistency() {
    std::vector<int> global_snapshot = this->nodes[0].snapshots.back();
    for(int i=0; i<this->number_of_nodes;i++) {
        if(this->nodes[i].snapshots.back()[i] != global_snapshot[i]) {
            return false;
        }
    }
    return true;
}