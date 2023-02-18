#ifndef JMCH_HASHPAIR_H
#define JMCH_HASHPAIR_H

#include <functional>

namespace std {
    
    template<typename T1, typename T2>
    struct hash<std::pair<T1, T2>> {
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            std::size_t hash1 = std::hash<T1>{}(p.first);
            std::size_t hash2 = std::hash<T2>{}(p.second);
            // Return bitwise XOR of hashes.
            return (hash1 == hash2) ? hash1 : hash1 ^ hash2;
        }
    };
}

#endif
