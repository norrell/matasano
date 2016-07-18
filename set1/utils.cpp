#include <vector>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <cmath>

#include "typedefs.h"

std::vector<byte> hexstring_to_bin(const std::string &hexstr, size_t strlen_nonull)
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

    byte first, second;
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

std::string bin_to_hexstring(const std::vector<byte> &bin)
{
    // TODO
}

static std::string b64_encode(const std::vector<byte> &v){
    size_t len = v.size();
    size_t strsize = (size_t) (4 * std::ceil((double) len / 3.0));
    std::string str(strsize, '.');

    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

    int offset = 0;
    int i = 0;
    do {
        str[i++] = base64[(int) ((v[offset + 0] & 0xfc) >> 2)];

        if (len == 1) {
            str[i++] = base64[(int) ((v[offset + 0] & 0x03) << 4)];
            str[i++] = '=';
            str[i++] = '=';
            break;
        }

        str[i++] = base64[(int) (((v[offset + 0] & 0x03) << 4) | ((v[offset + 1] & 0xf0) >> 4))];

        if (len == 2) {
            str[i++] = base64[(int) ((v[offset + 1] & 0x0f) << 2)];
            str[i++] = '=';
            break;
        }

        str[i++] = base64[(int) (((v[offset + 1] & 0x0f) << 2) | ((v[offset + 2] & 0xc0) >> 6))];
        str[i++] = base64[(int) (v[offset + 2] & 0x3f)];
        offset += 3;
    } while (len -= 3);

    return str;
}

/*
 * Base64-encode a string
 */
std::string b64_encode(const std::string &str)
{
    std::vector<byte> v(str.begin(), str.end());
    return b64_encode(v);
}

/*
 * Base64-encode an open file
 */
std::string b64_encode(std::ifstream &file)
{
    /*
     * Currently reads entire file into string.
     * Not good for large files.
     * Prefer b64_encode(std::string) for now.
     */
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&contents[0], contents.size());

    return b64_encode(contents);
}

static std::string b64_decode(const std::vector<byte> &v)
{
    int unbase64[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52,
        53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0, -1, -1, -1,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
        42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1
    };

    int len = v.size();
    std::string str(len, '.');

    if (len & 0x03) {
        std::cerr << "String length must be an even multiple of 4" << std::endl;
        return std::string("");
    }

    int offset = 0;
    size_t strlen = 0;
    int k = 0;
    do {
        for (int i = 0; i <= 3; ++i) {
            if (v[offset + i] > 128 || unbase64[v[offset + i]] == -1) {
                std::cerr << "Invalid character for base64 encoding: " << (char) v[offset + i] << std::endl;
                return std::string("");
            }
        }

        str[k++] = (char) ((unbase64[v[offset + 0]] << 2) | ((unbase64[v[offset + 1]] & 0x30) >> 4));
        strlen++;

        if ((char) v[offset + 2] != '=') {
            str[k++] = (char) (((unbase64[v[offset + 1]] & 0x0f) << 4) | ((unbase64[v[offset + 2]] & 0x3c) >> 2));
            strlen++;
        }

        if ((char) v[offset + 3] != '=') {
            str[k++] = (char) (((unbase64[v[offset + 2]] & 0x03) << 6) | unbase64[v[offset + 3]]);
            strlen++;
        }
        offset += 4;
    } while (len -= 4);

    str.resize(strlen);

    return str;
}

/*
 * Base64-decode a string
 */
std::string b64_decode(const std::string &str)
{
    std::vector<byte> v(str.begin(), str.end());
    return b64_decode(v);
}

/*
 * Uses algorithm by Wegner (1960)
 */
static int calc_hamming_dist(byte c1, byte c2)
{
    int dist = 0;
    byte xord = c1 ^ c2;
    while (xord != 0) {
        dist++;
        xord &= xord - 1;
    }

    return dist;
}

int hamming_distance(const std::string &s1, const std::string &s2)
{
    if (s1.size() != s2.size()) {
        std::cerr << "Strings must be of equal length" << std::endl;
        return -1;
    }

    int dist = 0;
    for (unsigned int i = 0; i < s1.size(); ++i) {
        dist += calc_hamming_dist((byte) s1[i], (byte) s2[i]);
    }

    return dist;
}