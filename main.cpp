#include <iostream>
#include <cstring>
#include <array>
#include <list>
#include <fstream>

#include "singlebyte_xor.h"
#include "detect_singlebyte_xor.h"
#include "repeatkey_xor.h"
#include "utils.h"

#define BASE64
#define HEXBIN
#define SINGLEB_XOR
#define DETECT_SINGLEB_XOR
#define HAMMING_DIST
#define REPEAT_XOR

/*
 * Pass hexstring as argument in argv[1]
 */
int main(int argc, char *argv[])
{
#ifdef BASE64
    std::cout << "Testing base64 encoding... ";
    std::string hexstr1("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    std::string b64enc = b64_encode(hexstr1);

    if (b64enc == "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t")
        std::cout << "TEST PASSED" << std::endl;
    else
        std::cout << "TEST FAILED" << std::endl;

    /**********************************************/

    std::cout << "\nTesting base64 decoding... ";
    std::string textstr1 = b64_decode("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");

    if (textstr1 == "I'm killing your brain like a poisonous mushroom")
        std::cout << "TEST PASSED" << std::endl;
    else
        std::cout << "TEST FAILED" << std::endl;
#endif

#ifdef HEXBIN
    std::cout << "\nTesting binary to hex string conversion... ";

    std::string hexstr2("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    std::vector<byte> binstr = hex_to_bin(hexstr2);
    std::string backtohex = bin_to_hex(binstr);

    if (backtohex == hexstr2)
        std::cout << "TEST PASSED" << std::endl;
    else
        std::cout << "TEST FAILED" << std::endl;
#endif

#ifdef SINGLEB_XOR
    std::cout << "\nTesting single-byte XOR..." << std::endl;
    std::string cipher("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
    std::cout << "...ciphertext: " << cipher << std::endl;
    std::list<Plaintext> rank1 = singlebyte_xor(cipher);

    std::cout << "...plaintext candidates: " << std::endl;
    int i1 = 1;
    for (Plaintext p : rank1) {
        std::cout << "......(" << i1 << ") \"" << p.plaintext << '\"' << std::endl;
        std::cout << "..........(score = " << p.score << ", key = " << p.key << ")" << std::endl;
        ++i1;
    }
#endif

#ifdef DETECT_SINGLEB_XOR
    std::cout << "\nTesting detect single-byte XOR... " << std::endl;
    std::string filename("4.txt");
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }
    std::cout << "...ciphertext: \"" << filename << '\"' << std::endl;

    std::list<Plaintext> rank2 = detect_singlebyte_xor(file);
    std::cout << "...plaintext candidates: " << std::endl;
    int i2 = 1;
    for (Plaintext p : rank2) {
        std::cout << "......(" << i2 << ") \"" << p.plaintext << '\"' << std::endl;
        std::cout << "..........(score = " << p.score << ", key = " << p.key << ", line = " << p.linenumber << ")" << std::endl;
        ++i2;
    }
#endif

#if 0
    std::cout << "Testing base64 encoding..." << std::endl;
    std::string str("Man is distinguished");
    std::string b64str = b64_encode_text(str);

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

#ifdef REPEAT_XOR
    std::cout << "\nTesting repeating-key XOR... ";

    std::string textstr2("Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal");
    std::string encr2 = repeatkey_xor(textstr2, "ICE");

    if (encr2 == "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f")
        std::cout << "TEST PASSED" << std::endl;
    else
        std::cout << "TEST FAILED" << std::endl;
#endif

#ifdef HAMMING_DIST
    std::cout << "\nTesting hamming distance... ";
    int hamming = hamming_distance(std::string("this is a test"), std::string("wokka wokka!!!"));

    if (hamming == 37)
        std::cout << "TEST PASSED" << std::endl;
    else
        std::cout << "TEST FAILED" << std::endl;
#endif

    return 0;
}
