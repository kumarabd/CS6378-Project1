#include<iostream>
#include<string>
#include <iterator>
#include <algorithm>
#include <vector>
#include<pthread.h>
#include <unistd.h>
#include<sys/socket.h>
#include<sys/types.h> 
#include<cstring>
#include <stdio.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define clock 1000000

typedef struct {
    int sock;
    struct sockaddr sock_addr;
    int addr_len;
} connection_t;

typedef struct {
    int type;
    char * data;
    int source;
} message;

class Channel {
    private:
        int size;
        int server_fd;
        struct sockaddr_in socket_address;
    public:
        Channel();
        Channel(std::string h, int p);
        void start_socket();
        void send_socket(struct sockaddr_in serv_addr, std::string msg);
        struct sockaddr_in address();
        int fd();
};

class Node {
    private:
        int id;
        bool active_status, run;
        // bool message_type; //0->control message and 1->applications message
        Channel channel;
        int maxNumber;
        int minSendDelay;
        int minPerActive;
        int maxPerActive;
    public:
        std::vector< std::vector<int> > states;
        std::vector<Node*> neighbours;
        std::vector<int> marker_pending;
        bool marker_cycle;
        Node();
        Node(int id, std::string h, int p, int mn, int mipa, int mapa, int msd);
        int get_id();
        bool process_message(message msg);
        void send_message(Node * node, message msg);
        void add_neighbour(std::vector<int> neighbours);
        struct sockaddr_in get_address();
        void record_clock_value(std::vector<int> value);
        bool verify_clock(std::vector<int> value);
        void info();
        void sending_snapshot();
        void listen();
};
