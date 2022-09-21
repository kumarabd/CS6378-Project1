#include<iostream>
#include<string>
#include<list>
#include<sys/socket.h>
#include<sys/types.h> 
#include<netinet/in.h>

class Channel {
    private:
        int size;
        int server_fd;
        struct sockaddr_in socket_address;
    public:
        Channel();
        Channel(std::string h, int p);
        void start_socket();
};

class Node {
    private:
        int id;
        bool active_status;
        Channel channel;
        std::list<Node> neighbours;
    public:
        Node();
        Node(int id, std::string h, int p);
        void add_neighbours(int id, int val);
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
        Network(int mipa, int mapa, int msd, int mn, int sd);
        void add_nodes(std::list<Node> n);
};