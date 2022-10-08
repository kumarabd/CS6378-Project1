#include "node.h"

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

struct sockaddr_in Channel::address() {
    return this->socket_address;
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

void Channel::send_socket(struct sockaddr_in serv_addr, char* message) {
    int sock = 0, valread, client_fd;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
 
    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    send(sock, message, strlen(message), 0);

    close(client_fd);
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
    char* message;
    int newSocket;
    channel.start_socket(&newSocket);
    // Listen for messages
    while(1) {
        recv(newSocket, &message, sizeof(message), 0);
        printf("message receieved: %d",message);
        // Active node send message to random node
        // if not then
        // Remove the node from network if the node reach maxNumber of messages
        this->active_status = true;
        bool status = this->process_message(1, message);
        if(!status) {
            break;
        }
        this->active_status = false;
    }
        }
        

int Node::get_id() {
    return id;
}

bool Node::process_message(bool message_type, char *message) {
    int random_msg_number = this->minPerActive + ( std::rand() % (this->maxPerActive - this->minPerActive + 1) );
    random_msg_number = std::min<int>(random_msg_number, this->neighbours.size());
    printf("Sending message to node: %d\n", this->get_id());
    if (message_type == 1){
        std::string temp(message);
        std::vector<int> final_vector(temp.begin(), temp.end());
        this->snapshots.push_back(final_vector);
        for(int i=0; i<random_msg_number; i++) {
        this->send_message(this->neighbours[i], message_type);
        this->maxNumber--;
        if(this->maxNumber == 0) {
            return false;
        }
    }
    return true;
    }
    else if(message_type == 0){
        for(int i=0; i<neighbours.size(); i++) {
        this->send_message(this->neighbours[i], message_type);
    }
    return true;
    } 
}

struct sockaddr_in Node::get_address(){
    return this->channel.address();
}

void Node::send_message(Node node, bool message_type){
    if (message_type == 1){
        usleep(this->minSendDelay);
    }
    struct sockaddr_in serv_addr = node.get_address();
    std::vector<int> curr_state = this->snapshots.back();
    std::string str(curr_state.begin(), curr_state.end());
    char* message = const_cast<char*>(str.c_str());
    this->channel.send_socket(serv_addr, message);
}

void Node::record_clock_value(std::vector<int> value) {
    this->snapshots.push_back(value);
}

bool Node::verify_clock(std::vector<int> value) {
    return value == this->snapshots.back();
}
