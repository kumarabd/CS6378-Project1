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
    Node * node_addr;
};

void create_nodes(node_args *args) {
    struct node_args *as = args;
    Node node_obj = Node(as->id, as->host, as->port);
    as->node_addr = &node_obj;
}

int main()
{
    printf("Reading config\n");
	ReadConfig config = ReadConfig();
    config.read_config();

    printf("Gathering nodes\n");
    std::list<Node*> node_list;
    pthread_t * threads = new pthread_t[config.node];
    for(int i=0; i<config.node; i++) {
        // Create thread
        Node node_obj;
        struct node_args args = { .id = i, .host = config.hostNames[i], .port = config.ports[i], .node_addr = &node_obj };
        if(pthread_create(&threads[i], NULL, (void* (*)(void*))  (&create_nodes), (void *) &args)) {
            perror("thread create failed");
            exit(EXIT_FAILURE);
        }
        usleep(clock);
        node_list.push_back(&node_obj);
    }

    // create a network
    printf("Creating network\n");
    Network network = Network(config.minPerActive, config.maxPerActive, config.minSendDelay, config.snapshotDelay, config.maxNumber);
    network.add_nodes(node_list);
    printf("Network created\n");

    // run topology
    printf("Run topology");
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