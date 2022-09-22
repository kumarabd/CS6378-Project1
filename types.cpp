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

void Channel::start_socket(int * newSocket) {
    // 3 is the max queue limit
    if (listen(server_fd, 3) < 0) {
        perror("listening");
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(socket_address);
    *newSocket = accept(server_fd, (struct sockaddr*)&socket_address, &addr_size);
}

void Channel::send_socket() {
    if (connect(this->server_fd, (struct sockaddr *)&this->socket_address, sizeof(this->socket_address))) {
        perror("sending");
        exit(EXIT_FAILURE);
    }

    close(this->server_fd);
}

Node::Node() {};

Node::Node(int id, std::string h, int p, int mn, int mipa, int mapa, int msd) {
    printf("Creating Node: %d\n", id);
    id = id;
    maxNumber = mn;
    minPerActive = mipa;
    maxPerActive = mapa;
    minSendDelay = msd;
    active_status = false;
    channel = Channel(h, p);

    // start node server
    printf("Starting socket for node: %d\n", id);
    int message = 0;
    int newSocket;
    channel.start_socket(&newSocket);
    // Listen for messages
    while(1) {
        recv(newSocket, &message, sizeof(message), 0);
        printf("message receieved: %d",message);
        // Active node send message to random node
        // if not then
        // Remove the node from network if the node reach maxNumber of messages
        if(this->limit()) {
            this->active_status = false;
        } else{
            std::list<Node> nb = this->get_neighbours();
            int num = rand() % nb.size(); // NOTE: Check if the random number becomes itself
            auto beg = nb.begin();
            std::advance(beg, num);
            Node snode = *beg;
            snode.send_message();
            this->iterate_max_number();
        }
        // Wait for minSendDelay after sending the message
        usleep(minSendDelay);
    }
}

int Node::get_id() {
    return id;
}

void Node::send_message() {
    // Client implementation
    printf("Sending message to node: %d\n", this->get_id());
    this->channel.send_socket();
}

void Node::listen() {
    while(1) {
    }
}

void Node::add_neighbours(int id, int val) {
    // Add here
}

std::list<Node> Node::get_neighbours() {
    return this->neighbours;
}

void Node::iterate_max_number() {
    this->maxNumber--;
}

bool Node::limit() {
    if(this->maxNumber == 0) {
        return true;
    }
    return false;
}

Network::Network(int sd) {
    number_of_nodes = 0;
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
    while(1) {
        if(flag) {
            // Setting random node to active
            int num = rand() % number_of_nodes;
            auto beg = nodes.begin();
            std::advance(beg, num);
            Node * curr_node = *beg;
            curr_node->send_message();
            flag = false;
        }
        
        // Stop if everything is passive || the list is empty
        if(nodes.size() == 0) {
            break;
        }
    }
}