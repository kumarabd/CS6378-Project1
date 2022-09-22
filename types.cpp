#include "types.h"

Channel::Channel() {};

Channel::Channel(std::string h, int p) {
    printf("Creating socket\n");
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       perror("socket failed");
       exit(EXIT_FAILURE);
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(p);
    if (bind(server_fd, (struct sockaddr*) &socket_address, sizeof(socket_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Channel::start_socket() {
    // 3 is the max queue limit
    if (listen(server_fd, 3) < 0) {
        perror("listening");
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(socket_address);
    int newSocket = accept(server_fd, (struct sockaddr*)&socket_address, &addr_size);
    int message = 0;
    recv(newSocket, &message, sizeof(message), 0);
    printf("message receieved: %d",message);
}

Node::Node() {};

Node::Node(int id, std::string h, int p, int mn) {
    printf("Creating Node: %d\n", id);
    id = id;
    maxNumber = mn;
    active_status = false;
    channel = Channel(h, p);

    // start node server
    printf("Starting socket for node: %d\n", id);
    channel.start_socket();
}

int Node::get_id() {
    return id;
}

void Node::send_message(Node * n) {
    printf("Sending message to node: %d\n", n->get_id());
}

void Node::add_neighbours(int id, int val) {
    // Add here
}

std::list<Node> Node::get_neighbours() {
    return this->neighbours;
}

void Node::iterate_max_number() {
    this->maxNumber++;
}

Network::Network(int mipa, int mapa, int msd, int sd) {
    number_of_nodes = 0;
    minPerActive = mipa;
    maxPerActive = mapa;
    minSendDelay = msd;
    snapshotDelay = sd;
}

void Network::add_nodes(std::list<Node*> ns) {
    printf("Adding nodes to the network\n");
    nodes = ns;
    number_of_nodes = nodes.size();
}


void Network::run() {
    printf("Running the network");
    bool flag = true;
    while(flag) {
        // Setting random node to active
        int num = rand() % number_of_nodes;
        auto beg = nodes.begin();
        std::advance(beg, num);
        Node * curr_node = *beg;

        // Active node send message to random node
        num = rand() % curr_node->get_neighbours().size(); // NOTE: Check if the random number becomes itself
        beg = nodes.begin();
        std::advance(beg, num);
        curr_node->send_message(*beg);
        curr_node->iterate_max_number();
        
        // Remove the node from network if the node reach maxNumber of messages
    }
}