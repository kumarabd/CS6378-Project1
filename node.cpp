#include "node.h"

std::string convertToString(char* a, int size)
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

std::vector<int> intersection(std::vector<int> v1,
                                      std::vector<int> v2){
    std::vector<int> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

Channel::Channel() {};

Channel::Channel(std::string h, int p) {
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

void Channel::send_socket(struct sockaddr_in serv_addr, std::string msg) {
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
    send(sock, msg.c_str(), strlen(msg.c_str()), 0);
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

        std::string delimiter = "///";
        std::string data = convertToString(buffer, sizeof(buffer)/sizeof(char));
        std::string source = data.substr(0, data.find(delimiter));
        std::string content = data.substr(1, data.find(delimiter));
        std::string type = data.substr(2, data.find(delimiter));
        // eg: "0///<data>///0"
        // Active node send message to random node
        // if not then
        // Remove the node from network if the node reach maxNumber of messages
        this->active_status = true;
        message msg = { atoi(type.c_str()), const_cast<char*>(content.c_str()), atoi(source.c_str())};
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

bool Node::process_message(message msg) {
    int recepient_size;
    std::vector<Node*> recepients;
    std::vector<int>::iterator position;
    if(msg.source >= 0 && !this->marker_cycle) {
        this->marker_pending.push_back(msg.source);
    }
    // Application messages
    if (msg.type){
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
    else {
        recepient_size = this->neighbours.size();
        recepients = this->neighbours;
    }
    
    if(this->marker_pending.size() == 0) {
        this->marker_cycle = false;
    }
    if(this->marker_cycle) {
        position = std::find(this->marker_pending.begin(),this->marker_pending.end(), msg.source);
        if(position != this->marker_pending.end()) {
            this->marker_pending.erase(position);
        } else {
            std::vector<int> recp_ids;
            for(int i=0; i<recepients.size(); i++) {
                recp_ids.push_back(recepients[i]->get_id());
            }
            std::vector<int> results = intersection(recp_ids, this->marker_pending);
            std::vector<Node *> final_nodes;
            for(int i=0; i<results.size(); i++) {
                position = std::find(this->marker_pending.begin(),this->marker_pending.end(), msg.source);
                int d = std::distance(this->marker_pending.begin(), position);
                if(position != this->marker_pending.end()) {
                    final_nodes.push_back(recepients.at(d));
                }
            }

            for(int i=0; i<final_nodes.size(); i++) {
                this->marker_pending.push_back(final_nodes[i]->get_id()); // Marking request message
                printf("Sending message of type %d from %d to node: %d\n", msg.type, this->get_id(), final_nodes[i]->get_id());
                this->send_message(final_nodes[i], msg);
            }
        }
    } else {
        for(int i=0; i<recepient_size; i++) {
            this->marker_pending.push_back(recepients[i]->get_id()); // Marking request message
            printf("Sending message of type %d from %d to node: %d\n", msg.type, this->get_id(), recepients[i]->get_id());
            this->send_message(recepients[i], msg);
        }
        this->marker_cycle = true;
    }
    usleep(clock*2);
}

struct sockaddr_in Node::get_address(){
    return this->channel.address();
}

void Node::send_message(Node * node, message msg){
    if (msg.type){
        usleep(this->minSendDelay);
    }
    std::vector<int> curr_state = this->states.back();
    curr_state[this->get_id()] += 1;
    this->record_clock_value(curr_state);

    struct sockaddr_in serv_addr = node->get_address();
    std::string str(curr_state.begin(), curr_state.end());
    char* message = const_cast<char*>(str.c_str());
    std::string m = std::to_string(this->get_id()) +"///";
    m = m + msg.data;
    m = m + "///";
    m = m + std::to_string(int(msg.type));
    this->channel.send_socket(serv_addr, m);
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