#include <fstream>
#include "../../include/jmch/getlines.h"

using namespace jmch;
using namespace std;

List<string> jmch::getlines(int argc, char** argv) {
    if (argc != 2) throw invalid_argument("Supply input file.");
            
    ifstream file(argv[1]);
    if (!file.is_open()) throw fstream::failure("Invalid input file.");

    List<string> lines;
    string line;

    while (getline(file, line)) lines.pushBack(line);
    file.close();

    return lines;
}
