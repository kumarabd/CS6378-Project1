#include <iostream>
#include <cstdlib>
#include <fstream>
#include "readConfig.cpp"
#include "types.h"
using namespace std;

int main()
{
    printf("Reading config\n");
    // int n = 6;
    // int minPerActive = 2;
    // int maxPerActive = 4;
    // int minSendDelay = 1000;
    // int snapshotDelay = 1000;
    // int maxNumber = 3;
    // std::string node_host[6] = {"localhost", "localhost", "localhost", "localhost", "localhost", "localhost"};
    // int node_port[6] = {8000, 8001, 8002, 8003, 8004, 8005};
	ReadConfig readconfig = ReadConfig();
    readconfig.read_config();

    cout << readconfig.node << " " << readconfig.minPerActive << " " << readconfig.maxPerActive <<" " << readconfig.minSendDelay<<" " <<readconfig.snapshotDelay<< " " << readconfig.maxNumber << endl;
    cout << "print ports" << endl;
    for (auto port: readconfig.ports) cout << port << " "; cout << endl;

    cout << "print hostnames" << endl;
    for (auto hostName : readconfig.hostNames) cout << hostName << " "; cout << endl;

    cout << "neighbours" << endl;
    for (auto nei_list : readconfig.neighbors) {
        for (auto neighbor : nei_list) cout << neighbor << " "; 
        cout << endl;
        }

    // printf("Adding nodes\n");
    // std::list<Node> node_list;
    // for(int i=0; i<6; i++) {
    //     node_list.push_back(Node(i, node_host[i], node_port[i]));
    // }

    printf("Creating network\n");
    //Network network = Network(minPerActive, maxPerActive, minSendDelay, snapshotDelay, maxNumber);
    //network.add_nodes(node_list);

}