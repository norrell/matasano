#include <iostream>
#include <cstring>
#include <array>
#include <list>
#include <fstream>

#include "singlebyte_xor.h"
#include "detect_singlebyte_xor.h"

#define DETECT_SINGLEB_XOR
//#define SINGLEB_XOR

/*
 * Pass hexstring as argument in argv[1]
 */
int main(int argc, char *argv[])
{
#ifdef SINGLEB_XOR
    const char cipher[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    std::list<Plaintext> rank = singlebyte_xor(cipher, sizeof(cipher) - 1);

    for (Plaintext p : rank)
        std::cout << "Score = " << p.score << ", key = " << p.key << ", plaintext = \"" << p.plaintext << '\"' << std::endl;

#endif

#ifdef DETECT_SINGLEB_XOR
    std::ifstream file;
    file.open("4.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    std::list<Plaintext> rank = detect_singlebyte_xor(file);

    for (Plaintext p : rank)
        std::cout << '\"' << p.plaintext << "\" (score = " << p.score << ", key = " << p.key << ", line = " << p.linenumber << ")" << std::endl;
#endif

    return 0;
}
