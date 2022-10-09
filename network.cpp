#include "network.h"
#include "output.h"

std::vector<std::future<void>> pending_futures;

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
        this->nodes[id].marker_pending.push_back(&this->nodes[neighbours[i]]);
    }
}

void Network::run() {
    printf("Running the network\n");
    // Initialize states to 0
    std::vector<int> init_state(this->nodes.size(), 0);
    for(int i=0; i < this->nodes.size() ; i++) {
        this->nodes[i].states.push_back(init_state);
    }

    // Start the nodes
    for(int i =0; i< this->nodes.size();i++) {
        std::future<void> ft = std::async(std::launch::async, &Node::listen, this->nodes[i]);
        pending_futures.push_back(std::move(ft));
        //this->nodes[i].listen();
    }

    usleep(clock);

    // Send process messages to start with process 0
    message msg = { 0, "start" };
    this->nodes[0].process_message(msg);
    while(this->message_counter) {
        this->message_counter--;
    }

    // Verify consistency of global states
    //if(this->verify_consistency()) {
    //    printf("State consistent\n");
    //}else {
    //    printf("State inconsistent\n");
    //}
}

bool Network::verify_consistency() {
    std::vector<int> global_snapshot = this->nodes[0].states.back();
    for(int i=0; i<this->number_of_nodes;i++) {
        if(this->nodes[i].states.back()[i] != global_snapshot[i]) {
            return false;
        }
    }
    return true;
}

void Network::save() {
    for(int i=0; i<this->nodes.size(); i++) {
        generate_output(this->nodes[i].states, std::to_string(this->nodes[i].get_id()));
    }
}