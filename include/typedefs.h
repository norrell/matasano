#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <string>

typedef unsigned char byte;

struct Plaintext {
    int score;
    int key;
    int linenumber;
    std::string plaintext;
};

#endif /* TYPEDEFS_H */