#include<iostream>
#include<string>
#include<list>
#include <iterator>
#include<pthread.h>
#include <unistd.h>
#include<sys/socket.h>
#include<sys/types.h> 
#include<netinet/in.h>
#define clock 1000000

typedef struct {
    int sock;
    struct sockaddr sock_addr;
    int addr_len;
} connection_t;

class Channel {
    private:
        int size;
        int server_fd;
        struct sockaddr_in socket_address;
    public:
        Channel();
        Channel(std::string h, int p);
        void start_socket(int * s);
        void send_socket();
};

class Node {
    private:
        int id;
        bool active_status;
        Channel channel;
        int maxNumber;
        int minSendDelay;
        int minPerActive;
        int maxPerActive;
        std::list<Node> neighbours;
    public:
        Node();
        Node(int id, std::string h, int p, int mn, int mipa, int mapa, int msd);
        int get_id();
        void send_message();
        void listen();
        void add_neighbours(int id, int val);
        std::list<Node> get_neighbours();
        void iterate_max_number();
        bool limit();
};

class Network {
    private:
        int number_of_nodes;
        int snapshotDelay;
        std::list<Node*> nodes;
    public:
        Network(int sd);
        void add_nodes(std::list<Node*> n);
        void run();
};