#include "output.h"

extern void generate_output(std::vector< std::vector<int> > snapshots, std::string name) {
    printf("Creating output for %s\n", name.c_str());
    for(int i=0; i<snapshots.size(); i++) {
        std::string data = "";
        for(int j=0; j<snapshots[i].size(); j++) {
            std::vector<int> curr_state = snapshots[i];
            std::string str(curr_state.begin(), curr_state.end());
            data = str;
            printf("%s", data.c_str());
        }
        std::string file = "test1-"+name;
        file = file+".out";
        std::ofstream outfile(file.c_str());
        outfile << data;
        outfile << "\n";
        outfile.close();
    }
}