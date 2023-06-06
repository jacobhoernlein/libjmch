#ifndef JMCH_GETLINES_H
#define JMCH_GETLINES_H

#include <string>
#include "Array.cpp"

namespace jmch {
    
    /// @brief Get the lines of the file named in argv[1] as a List.
    /// @param argc The size of argv.
    /// @param argv An array of character pointers.
    /// @return A List of strings representing the lines of the file.
    /// @exception std::argument_error if argc < 2.
    /// @exception std::ifstream::failure if the file doesn't exist.
    Array<std::string> getlines(int argc, char** argv);
}

#endif
