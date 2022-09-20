#include<iostream>
#include<string>
#include<list>

class Channel {
    private:
        std::string host;
        int port;
    public:
        Channel(){};
        Channel(std::string h, int p) {
            host = h;
            port = p;
        }

        void start_server() {
            
        }
};

class Node {
    private:
        int id;
        bool active_status;
        Channel channel;
        std::list<Node> neighbours;
    public:
        Node(){};
        Node(int id, std::string h, int p) {
            id = id;
            active_status = false;
            channel = Channel(h, p);

            // start node server
            channel.start_server();
        }

        void add_neighbours(int id, int val) {
            // Add neighbours
        }
};

class Network {
    private:
        int number_of_nodes;
        int minPerActive;
        int maxPerActive;
        int minSendDelay;
        int maxNumber;
        int snapshotDelay;
        std::list<Node> nodes;
    public:
        Network(int n, int mipa, int mapa, int msd, int mn, int sd) {
            number_of_nodes = n;
            minPerActive = mipa;
            maxPerActive = mapa;
            minSendDelay = msd;
            maxNumber = mn;
            snapshotDelay = sd; 
        }
};