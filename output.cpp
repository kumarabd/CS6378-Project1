#include "output.h"

void generate_output(int n) {
    for(int i=0; i<n; i++) {
        std::string file = "build/test2-"+std::to_string(i);
        file = file+".out";
        std::ofstream outfile(file.c_str());
    }
}