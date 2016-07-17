#include <vector>
#include <cstddef>
#include <iostream>

#include "typedefs.h"

std::vector<byte> hexstring_to_bin(std::string hexstr, size_t strlen_nonull)
{
    std::vector<byte> bin(strlen_nonull / 2, (byte) 0);

    if (strlen_nonull % 2) {
        std::cerr << "Even number of hex digits required" << std::endl;
        return bin;
    }

    const byte hashmap[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  !"#$%&'
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ()*+,-./
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
        0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ[\]^_
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };     // `abcdef

    byte first;
    byte second;
    for (unsigned int i = 0; i < strlen_nonull - 1; i += 2) {
        first = hashmap[(int) hexstr[i]];
        second = hashmap[(int) hexstr[i + 1]];
        bin[i / 2] = (byte) ((first << 4) | second);
    }

    return bin;
}

std::vector<byte> hexstring_to_bin(const char *hexstr, size_t strlen_nonull)
{
    return hexstring_to_bin(std::string(hexstr, strlen_nonull), strlen_nonull);
}

std::string bin_to_hexstring(std::vector<byte> bin)
{
    // TODO
}

std::string b64_encode(std::vector<byte> v){
    // TODO
}

std::string b64_decode(std::string str) {
    // TODO
}

int calc_hamming_dist(std::string s1, std::string s2)
{
    // TODO
}