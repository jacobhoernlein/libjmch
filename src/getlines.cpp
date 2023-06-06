#include <fstream>
#include "../include/getlines.h"

using namespace std;

namespace jmch {
    Array<string> getlines(int argc, char** argv) {
        if (argc != 2) throw invalid_argument("Supply input file.");
                
        ifstream file(argv[1]);
        if (!file.is_open()) throw fstream::failure("Invalid input file.");

        Array<string> lines;
        string line;

        while (getline(file, line))
            lines.pushBack(line);
        
        file.close();
        return lines;
    }
}
