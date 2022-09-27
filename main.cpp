#include <iostream>
#include <cstdlib>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include "readConfig.cpp"
#include "types.h"
#include "output.h"
using namespace std;

struct node_args {
    int id;
    std::string host;
    int port;
    int mn;
    Node * node_addr;
    int mipa;
    int mapa;
    int msd;
};

void create_nodes(node_args *args) {
    struct node_args *as = args;
    Node node_obj = Node(as->id, as->host, as->port, as->mn, as->mipa, as->mapa, as->msd);
    as->node_addr = &node_obj;
}

int main()
{
    printf("Reading config\n");
	ReadConfig config = ReadConfig();
    config.read_config();

    printf("Gathering nodes\n");
    std::vector<Node*> node_list;
    pthread_t * threads = new pthread_t[config.node];
    for(int i=0; i<config.node; i++) {
        // Create thread
        Node node_obj;
        struct node_args args = { .id = i, .host = config.hostNames[i], .port = config.ports[i], .mn = config.maxNumber , .node_addr = &node_obj, .mipa = config.minPerActive, .mapa = config.maxPerActive, .msd = config.minSendDelay};
        if(pthread_create(&threads[i], NULL, (void* (*)(void*))  (&create_nodes), (void *) &args)) {
            perror("thread create failed");
            exit(EXIT_FAILURE);
        }
        usleep(clock);
        node_list.push_back(&node_obj);
    }

    // create a network
    printf("Creating network\n");
    Network network = Network(config.snapshotDelay);
    network.add_nodes(node_list);
    for(int i=0; i<config.neighbors.size(); i++) {
        network.add_neighbour(i, config.neighbors[i]);
    }
    printf("Network created\n");

    // run topology
    network.run();

    // Generate output
    //generate_output(config.node);

     // wait for the threads
    void *status;
    for(int i=0; i<config.node; i++) {
        pthread_join(threads[i], &status);
        printf("node status: %d\n",*(int*)status);
    }
}