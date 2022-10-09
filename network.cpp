#include "network.h"
#include <queue>
#include <vector>
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

void Network::construct_mst() {
    // fill up parent and children ids of all Nodes in the network
    std::vector<Node> ns = this->nodes;
    std::vector <bool> visited(ns.size(),false);
    std::queue <int> q;
    q.push(0);
    visited[0] = true;
    int currParent = -1;
    
    while(!q.empty()) {
        int currNode = q.front();
        q.pop();
        if (currNode == 0) {
            ns[currNode].setParent(-1);
            std::vector <int> children;
            for (int i=0; i<(ns[currNode].neighbours).size(); i++) {
                int currNeigh = ((ns[currNode].neighbours)[i])->get_id();
                if (currNeigh != currParent) {
                    if (!visited[currNeigh]) {
                        children.push_back(currNeigh);
                        ns[currNeigh].setParent(currNode);
                        visited[currNeigh] = true;
                        q.push(currNeigh);
                    }
                }
            }
            ns[currNode].setChildren(children);
        } else {
            std::vector <int> children;
            for (int i=0; i<(ns[currNode].neighbours).size(); i++) {
                int currNeigh = ((ns[currNode].neighbours)[i])->get_id();
                if (currNeigh != currParent) {
                    if (!visited[currNeigh]) {
                        children.push_back(currNeigh);
                        ns[currNeigh].setParent(currNode);
                        visited[currNeigh] = true;
                        q.push(currNeigh);
                    }
                }
            }
            ns[currNode].setChildren(children);
        }
    }
    // printing out parents and children list for verification
    std::cout << "The parents list for the network is: " << std::endl;
    for (Node node: ns) {
        std::cout << node.getParent() << " "; 
        std::cout << std::endl;
    } 

    for (Node node: ns) {
        std::cout << "Children of " << node.get_id() << " are ";
        for (auto child: node.getChildren()) std::cout << child << " ";
        std::cout << std::endl;
    }
}


void Network::add_neighbour(int id, std::vector<int> neighbours) {

    for(int i=0; i<neighbours.size(); i++) {
<<<<<<< HEAD
        std::cout << neighbours[i] << (this->nodes).size()<< std::endl;
        this->nodes[id].neighbours.push_back(&(this->nodes[neighbours[i]]));
=======
        this->nodes[id].neighbours.push_back(&this->nodes[neighbours[i]]);
        this->nodes[id].marker_pending.push_back(&this->nodes[neighbours[i]]);
>>>>>>> master
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