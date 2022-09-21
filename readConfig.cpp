#include "readConfig.h"

int str_to_int(string s){
    int num = 0;
    for (int i = 0; i < s.length(); i++){
        char c = s[i];
        if (c == ' '){
            continue;
        } else {
            num*=10;
            num+=c-'0';
        } 
    }
    return num;
}

ReadConfig::ReadConfig() {};

void ReadConfig::read_config() {
    ifstream testFile("test1.txt");
    if (testFile.is_open()){

        while(getline(testFile, line)){
            line.erase(remove_if(line.begin(), line.end(), ::isspace),line.end());
            if(line[0] == '#' || line.empty()) {
                    continue;
            } 

            stringstream ss(line);
            getline(ss, nodestr, ',');
            getline(ss, minPerActivestr, ',');
            getline(ss, maxPerActivestr, ',');
            getline(ss, minSendDelaystr, ',');
            getline(ss, snapshotDelaystr, ',');
            getline(ss, maxNumberstr, ',');

            node = str_to_int(nodestr);
            minPerActive = str_to_int(minPerActivestr);
            maxPerActive = str_to_int(maxPerActivestr);
            minSendDelay = str_to_int(minSendDelaystr);
            snapshotDelay = str_to_int(snapshotDelaystr);
            maxNumber = str_to_int(maxNumberstr);
            break;
        }

        for (int i = 0; i < node; i++) {
            getline(testFile, line);
            if(line[0] == '#' || line.empty()) {
                i--;
                continue;
            } 
            else if (line.find('#') != string::npos) {
                line = line.substr(0,line.find('#'));
            } 
            int nodeid, portno;
            string nodeidstr, portnostr, hostname;
            stringstream ss(line);
            ss >> nodeidstr;
            ss >> hostname;
            ss >> portnostr;

            nodeid = str_to_int(nodeidstr);
            portno = str_to_int(portnostr);
            hostNames.push_back(hostname);
            ports.push_back(portno);

        }
        for (int i = 0; i < node; i++) {
            getline(testFile, line);
            if(line[0] == '#' || line.empty()) {
                i--;
                continue;
            } else if (line.find('#') != string::npos) {
                line = line.substr(0,line.find('#'));
            }
            string nodeIdStr;
            stringstream ss(line);
            vector <int> curr_neighbors;
            while(ss >> nodeIdStr) {
                int nodeId = str_to_int(nodeIdStr);
                curr_neighbors.push_back(nodeId);
            }
            neighbors.push_back(curr_neighbors);

        }
        
     } else{
        cerr << "File failed to open" << endl;
    }
}