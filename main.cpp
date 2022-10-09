#include <iostream>
#include <cstdlib>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include "readConfig.cpp"
#include "network.h"
using namespace std;

typedef struct thread_data {
    int id;
    std::string host;
    int port;
    int mn;
    int mipa;
    int mapa;
    int msd;
} thread_data;

std::vector<Node> nodes;

void * create_nodes(thread_data* args) {
    thread_data *tdata = (thread_data *)args;
    Node addr = Node(tdata->id, tdata->host, tdata->port, tdata->mn, tdata->mipa, tdata->mapa, tdata->msd);
    nodes.push_back(addr);
    return NULL;
}

int main()
{
    printf("Reading config\n");
	ReadConfig config = ReadConfig();
    config.read_config();

    printf("Gathering nodes\n");
    std::vector<Node*> node_list;
    pthread_t * threads = new pthread_t[config.node];
    thread_data * args = new thread_data[config.node];
    for(int i=0; i<config.node; i++) {
        // Create thread
        args[i].id = i;
        args[i].host = config.hostNames[i];
        args[i].port = config.ports[i];
        args[i].mn = config.maxNumber;
        args[i].mipa = config.minPerActive;
        args[i].mapa = config.maxPerActive;
        args[i].msd = config.minSendDelay;

        if(pthread_create(&threads[i], NULL, (void* (*)(void*))  (&create_nodes), (void *) &args[i])) {
            perror("thread create failed");
            exit(EXIT_FAILURE);
        }
        usleep(clock);
        //args->node_addr->info();
    }

    // create a network
    printf("Creating network\n");
    Network network = Network(config.snapshotDelay);
    network.add_nodes(nodes);
    for(int i=0; i<config.neighbors.size(); i++) {
        network.add_neighbour(i, config.neighbors[i]);
    }
    printf("Network created\n");

    // run topology
    network.run();
    network.save();
    
    // wait for the threads
    void *status;
    for(int i=0; i<config.node; i++) {
        pthread_join(threads[i], &status);
        //printf("node status: %d\n",*(int*)status);
    }

    return 0;
}