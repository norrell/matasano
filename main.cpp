#include <iostream>
#include <cstring>
#include <array>
#include <list>
#include <fstream>

#include "singlebyte_xor.h"
#include "detect_singlebyte_xor.h"
#include "utils.h"

//#define DETECT_SINGLEB_XOR
//#define SINGLEB_XOR
//#define HAMMING_DIST
#define BASE64

/*
 * Pass hexstring as argument in argv[1]
 */
int main(int argc, char *argv[])
{
#ifdef SINGLEB_XOR
    std::cout << "Testing single-byte XOR..." << std::endl;
    const char cipher[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    std::list<Plaintext> rank = singlebyte_xor(cipher, sizeof(cipher) - 1);

    for (Plaintext p : rank)
        std::cout << "Score = " << p.score << ", key = " << p.key << ", plaintext = \"" << p.plaintext << '\"' << std::endl;

#endif

#ifdef DETECT_SINGLEB_XOR
    std::cout << "Detecting single-byte XOR..." << std::endl;

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

#ifdef BASE64
    std::cout << "Testing base64 encoding..." << std::endl;
    std::string str("Man is distinguished");
    std::string b64str = b64_encode(str);

    std::cout << b64str << std::endl;

    std::cout << "Testing base64 decoding..." << std::endl;
    std::string b64dec = b64_decode(b64str);

    std::cout << b64dec << std::endl;

    std::ifstream in("man.txt", std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    std::cout << b64_encode(in) << std::endl;
#endif

#ifdef HAMMING_DIST
    std::cout << "Test hamming distance..." << std::endl;
    int hamming = hamming_distance(std::string("this is a test"), std::string("wokka wokka!!!"));

    std::cout << "Hamming distance is " << hamming << std::endl;
#endif

    return 0;
}
