#include "node.h"

Channel::Channel() {};

Channel::Channel(std::string h, int p) {
    printf("Creating socket\n");
    // Create socket
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       perror("socket failed");
       exit(EXIT_FAILURE);
    }

    socket_address.sin_family = AF_INET;
    //socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(p);

    hostent* hostname = gethostbyname(h.c_str());
    std::string st = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
    //inet_aton(st.c_str(), &socket_address.sin_addr);
    if (inet_aton(st.c_str(), &socket_address.sin_addr) < 0) {
            perror("Address not recognized");
            exit(EXIT_FAILURE);
    }

    if (bind(this->server_fd, (struct sockaddr*) &socket_address, sizeof(socket_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in Channel::address() {
    return this->socket_address;
}

int Channel::fd() {
    return this->server_fd;
}

void Channel::start_socket() {
    // 3 is the max queue limit
    if (listen(this->server_fd, 3) < 0) {
        perror("listening");
        exit(EXIT_FAILURE);
    }
}

void Channel::send_socket(struct sockaddr_in serv_addr, message msg) {
    int sock = 0, valread, client_fd;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
 
    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    send(sock, msg.data, strlen(msg.data), 0);
    //close(client_fd);
}

Node::Node() {};

Node::Node(int id, std::string h, int p, int mn, int mipa, int mapa, int msd) {
    printf("Creating Node: %d\n", id);
    this->id = id;
    this->maxNumber = mn;
    this->minPerActive = mipa;
    this->maxPerActive = mapa;
    this->minSendDelay = msd;
    this->run = true;
    this->active_status = false;
    this->channel = Channel(h, p);
    std::vector<Node *> empty_list;
    this->neighbours = empty_list;
    
}

void Node::listen() {
    // start node server
    printf("Starting socket for node: %d\n", this->get_id());
    message msg;
    int newSocket;
    this->channel.start_socket();
    // Listen for messages
    while(this->run) {
        sockaddr_in addr = this->get_address();
        socklen_t addr_size = sizeof(addr);
        if ((newSocket = accept(this->channel.fd(), (struct sockaddr*)&addr, &addr_size)) < 0) {
            perror("unable to accept\n");
            exit(EXIT_FAILURE);
        }

        // read message
        char buffer[1024] = {0};
        int reader = read(newSocket, buffer, 1024);
        printf("message received: %s\n",buffer);

        // Active node send message to random node
        // if not then
        // Remove the node from network if the node reach maxNumber of messages
        this->active_status = true;
        msg = { 0, 0, "test"};
        bool status = this->process_message(msg);
        if(!status) {
            break;
        }
        this->active_status = false;
    }
}
        

int Node::get_id() {
    return id;
}

void Node::setChildren(std::vector <int> childrenIds) {
    this->childrenIds = childrenIds;
}

void Node::setParent(int parentId) {
    this->parentId = parentId;
}

std::vector<int> Node::getChildren() {
    return childrenIds;
}

int Node::getParent() {
    return parentId;
}
bool Node::process_message(message msg) {
    this->info();
    int recepient_size;
    std::vector<Node*> recepients;
    // Application messages
    if (msg.type == 0){
        int random_msg_number = this->minPerActive + ( std::rand() % (this->maxPerActive - this->minPerActive + 1) );
        random_msg_number = std::min<int>(random_msg_number, this->neighbours.size());
        random_msg_number = std::min<int>(random_msg_number, this->maxNumber);
        if(this->maxNumber < random_msg_number) {
            this->maxNumber = 0;
            this->run = false;
            printf("Dying\n");
        } else {
            this->maxNumber = this->maxNumber - random_msg_number;
        }

        std::string temp(msg.data);
        std::vector<int> final_vector(temp.begin(), temp.end());
        this->states.push_back(final_vector);
        recepient_size = random_msg_number;
        recepients = this->neighbours;
    }
    // Marker messages
    else if (msg.type == 1){
        recepient_size = this->marker_pending.size();
        recepients = this->marker_pending;
        if(this->clState == false) {
            this->clState = true;
            // send marker to all the neighbours
            //start noting all channel incoming activity from neighbours
        } else {
            recepient_size = 0;
            std::vector<Node*>emp(0);
            recepients = emp;
        }
    } else {
        assert(msg.type == 2);
        this->convergeChildrenCounter --;
        transitMsgs &= (msg.data[0]=='T');
        if (! this->convergeChildrenCounter) {
            // send msg to parent this->getParendId()
            // should have 2 followed by T/F
            message msg;
            if(transitMsgs) {
                msg = {0, this->get_id(), "T"};
            } else {
                msg = {0, this->get_id(), "F"};
            }   
            int parent_idx = -1;
            for (auto neighbour: neighbours) {
                parent_idx ++;
                if (neighbour->get_id() == this->getParent())
                    break;
            }
            this->send_message(neighbours[parent_idx], msg);
        }
        
        if(this->get_id() == 0) {
            //for root node
            if(!this->convergeChildrenCounter) {
                if (transitMsgs) {
                    //STOP THE ENTIRE APPLICATION
                } else {
                    // CONTINUE SENDING SNAPSHOT REQUESTS
                }
            }
        }

        // mark yourself as false for the next CL invocation
        this->clState = false;
        // transitmsgs should also be made true for the next invocation
        this->transitMsgs = true;
        
        recepient_size = 0;
        std::vector<Node*>emp(0);
        recepients = emp;
    }
    msg.senderId = this->get_id();
    for(int i=0; i<recepient_size; i++) {
        printf("Sending message of type %d from %d to node: %d\n", msg.type, this->get_id(), recepients[i]->get_id());
        this->send_message(recepients[i], msg);
    }
    return true;
}

struct sockaddr_in Node::get_address(){
    return this->channel.address();
}

void Node::send_message(Node * node, message msg){
    if (msg.type){
        usleep(this->minSendDelay);
    }
    struct sockaddr_in serv_addr = node->get_address();
    std::vector<int> curr_state = this->states.back();
    std::string str(curr_state.begin(), curr_state.end());
    char* message = const_cast<char*>(str.c_str());
    this->channel.send_socket(serv_addr, msg);
}

void Node::record_clock_value(std::vector<int> value) {
    this->states.push_back(value);
}

bool Node::verify_clock(std::vector<int> value) {
    return value == this->states.back();
}

void Node::info() {
    printf("id: %d\n",this->id);
    printf("active_status: %d\n",this->active_status);
    printf("maxNumber: %d\n",this->maxNumber);
    printf("minSendDelay: %d\n",this->minSendDelay);
    printf("minPerActive: %d\n",this->minPerActive);
    printf("maxPerActive: %d\n",this->maxPerActive);
    printf("Neighbours: %d\n", this->neighbours.size());
    printf("Marker pending: %d\n", this->marker_pending.size());
    //printf("channel: %d",this->channel);
}