#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>

using namespace std;

class ReadConfig
{
private:
    string nodestr, minPerActivestr, maxPerActivestr, minSendDelaystr, snapshotDelaystr, maxNumberstr, line;
public:
    int node, minPerActive, maxPerActive, minSendDelay, snapshotDelay, maxNumber;
    vector <string> hostNames;
    vector <int> ports;
    vector <vector <int> > neighbors;
    
    ReadConfig();
    void read_config();
};

