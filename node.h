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

class Channel {
    private:
        int size;
        int server_fd;
        struct sockaddr_in socket_address;
    public:
        Channel();
        Channel(std::string h, int p);
        void start_socket();
        void send_socket(struct sockaddr_in serv_addr, char* message);
        struct sockaddr_in address();
        int fd();
};

class Node {
    private:
        int id;
        bool active_status;
        // bool message_type; //0->control message and 1->applications message
        Channel channel;
        int maxNumber;
        int minSendDelay;
        int minPerActive;
        int maxPerActive;
        int parentId;
        std::vector <int> childrenIds;
    public:
        std::vector< std::vector<int> > snapshots;
        std::vector<Node*> neighbours;
        Node();
        Node(int id, std::string h, int p, int mn, int mipa, int mapa, int msd);
        int get_id();
        bool process_message(bool message_type, char *message);
        void send_message(Node * node, bool message_type);
        void add_neighbour(std::vector<int> neighbours);
        void setParent (int parentId);
        void setChildren (std::vector<int> parentId);
        int getParent ();
        std::vector<int> getChildren ();
        struct sockaddr_in get_address();
        void record_clock_value(std::vector<int> value);
        bool verify_clock(std::vector<int> value);
        void info();
        void sending_snapshot();
        void listen();
};
