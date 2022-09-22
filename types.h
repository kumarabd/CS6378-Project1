#include<iostream>
#include<string>
#include<list>
#include<pthread.h>
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
        int get_id();
        void send_message(Node node);
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
        std::list<Node*> nodes;
    public:
        Network(int mipa, int mapa, int msd, int mn, int sd);
        void add_nodes(std::list<Node*> n);
        void run();
};