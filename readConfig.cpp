#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>

using namespace std;

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

void read_config(){
    ifstream testFile("test3.txt");
    if (testFile.is_open()){
    
        string nodestr, minPerActivestr, maxPerActivestr, minSendDelaystr, snapshotDelaystr, maxNumberstr, line;
        int node, minPerActive, maxPerActive, minSendDelay, snapshotDelay, maxNumber;
        
        vector <string> hostNames;
        vector <int> ports;

        vector <vector <int> > neighbors;
        // TODO: ignore lines with only spaces
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

            cout << node << " " << minPerActive << " " << maxPerActive <<" " << minSendDelay<<" " <<snapshotDelay<< " " << maxNumber <<endl;

            break;
        }
        // TODO: handle leading and trailing spaces 
        for (int i = 0; i < node+1; i++) {
            getline(testFile, line);
            cout << line << endl <<flush;
            if(line[0] == '#' || line.empty()) {
                i--;
                continue;
            } 
            else if (line.find('#') != string::npos) {
                line = line.substr(0,line.find('#'));
                cout << line << endl<<flush;
            } 
            int nodeid, portno;
            string nodeidstr, portnostr, hostname;
            stringstream ss(line);

            ss >> nodeidstr;
            cout << nodeidstr << endl << flush;
            
            ss >> hostname;
            cout << hostname << endl << flush;

            ss >> portnostr;
            cout << portnostr << endl << flush;

            nodeid = str_to_int(nodeidstr);
            portno = str_to_int(portnostr);

            cout << nodeid << " " << hostname << " " << portno << endl;

            hostNames.push_back(hostname);
            ports.push_back(portno);

        }
        for (int i = 0; i < node+1; i++) {
            getline(testFile, line);
            if(line[0] == '#' || line.empty()) {
                i--;
                continue;
            } else if (line.find('#') != string::npos) {
                line = line.substr(0,line.find('#'));
            }
            /*
                [[1,4],[0,2,3],[1,3],[1,2,4]..]
            */
            string nodeIdStr;
            stringstream ss(line);
            vector <int> curr_neighbors;
            while(ss >> nodeIdStr) {
                int nodeId = str_to_int(nodeIdStr);
                curr_neighbors.push_back(nodeId);
            }
            neighbors.push_back(curr_neighbors);
        
        }
        
        cout << "print ports" << endl;
        for (auto port: ports) cout << port << " "; cout << endl;

        cout << "print hostnames" << endl;
        for (auto hostName : hostNames) cout << hostName << " "; cout << endl;

        cout << "neighbours" << endl;
        for (auto nei_list : neighbors) {
            for (auto neighbor : nei_list) cout << neighbor << " "; 
            cout << endl;
        }

     } else{
        cerr << "File failed to open" << endl;
    }
}





