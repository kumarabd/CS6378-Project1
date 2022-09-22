#include "output.h"

void generate_output(int n) {
    for(int i=0; i<n; i++) {
        std::string file = "build/test2-"+i;
        file = file+".out";
        std::ofstream outfile(file);
    }
}