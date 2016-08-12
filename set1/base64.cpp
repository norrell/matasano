#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "hexbin.h"
#include "base64.h"
#include "typedefs.h"

/****************************************/
/**************** ENCODE ****************/
/****************************************/

static const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

/**
 * Base64-encode an array of bytes
 */
std::string b64enc(const std::vector<byte>& v){
    size_t vlen = v.size();
    size_t enclen = (size_t) (4 * std::ceil((double) vlen / 3.0));
    std::string enc(enclen, '.');

    int offset = 0, i = 0;
    do {
        enc[i++] = base64[(int) ((v[offset + 0] & 0xfc) >> 2)];

        if (vlen == 1) {
            enc[i++] = base64[(int) ((v[offset + 0] & 0x03) << 4)];
            enc[i++] = '=';
            enc[i++] = '=';
            break;
        }

        enc[i++] = base64[(int) (((v[offset + 0] & 0x03) << 4) | ((v[offset + 1] & 0xf0) >> 4))];

        if (vlen == 2) {
            enc[i++] = base64[(int) ((v[offset + 1] & 0x0f) << 2)];
            enc[i++] = '=';
            break;
        }

        enc[i++] = base64[(int) (((v[offset + 1] & 0x0f) << 2) | ((v[offset + 2] & 0xc0) >> 6))];
        enc[i++] = base64[(int) (v[offset + 2] & 0x3f)];
        offset += 3;
    } while (vlen -= 3);

    return enc;
}

/**
 * Base64-encode a string of hexadecimal digits
 */
std::string b64enc_hex(const std::string& hexstr)
{
    std::vector<byte> v = hex2bin(hexstr);
    return b64enc(v);
}

/*
 * Base64-encode a character string
 */
std::string b64enc(const std::string& str)
{
    std::vector<byte> v(str.begin(), str.end());
    return b64enc(v);
}

/*
 * Base64-encode a file
 */
std::string b64enc(std::ifstream& file)
{
    /*
     * Currently reads entire file into string.
     * Not good for large files.
     */
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&contents[0], contents.size());

    return b64enc_hex(contents);
}

/****************************************/
/**************** DECODE ****************/
/****************************************/

static const int unbase64[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52,
    53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0, -1, -1, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1
};

/**
 * Base64-decode an array of bytes
 * \return string or vector<byte> ???? I think it should rather by a vector of bytes
 */
std::string b64dec(const std::vector<byte>& v)
{
    int vlen = v.size();
    std::vector<byte> dec(vlen, 0);
    size_t declen = 0;

    if (vlen & 0x03) {
        std::cerr << "String length must be an even multiple of 4" << std::endl;
        return std::string("");
    }

    int offset = 0;
    int k = 0;
    do {
        for (int i = 0; i <= 3; ++i) {
            if (v[offset + i] > 128 || unbase64[v[offset + i]] == -1) {
                std::cerr << "Invalid character for base64 encoding: " << (char) v[offset + i] << std::endl;
                return std::string("");
            }
        }

        dec[k++] = (byte) ((unbase64[v[offset + 0]] << 2) | ((unbase64[v[offset + 1]] & 0x30) >> 4));
        declen++;

        if ((char) v[offset + 2] != '=') {
            dec[k++] = (byte) (((unbase64[v[offset + 1]] & 0x0f) << 4) | ((unbase64[v[offset + 2]] & 0x3c) >> 2));
            declen++;
        }

        if ((char) v[offset + 3] != '=') {
            dec[k++] = (byte) (((unbase64[v[offset + 2]] & 0x03) << 6) | unbase64[v[offset + 3]]);
            declen++;
        }
        offset += 4;
    } while (vlen -= 4);

    dec.resize(declen);

    return std::string(dec.begin(), dec.end());
}

/**
 * Base64-decode a character string
 */
std::string b64dec(const std::string& str)
{
    std::vector<byte> v(str.begin(), str.end());
    return b64dec(v);
}

/**
 * Base64-decode an open file
 */
#if 0
void b64dec(const std::ifstream& from, const std::ofstream& to)
{
    for (std::string line; std::getline(from, line); ) {
        std::string decline = b64dec(line);
        to.write(decline.c_str(), decline.size());
    }
}
#endif

/**
 * Base64-decode a file
 */
void b64dec(const std::string &from, const std::string &to)
{
    std::ifstream ffrom(from);
    if (!ffrom.is_open()) {
        std::cerr << "Could not open input file" << std::endl;
        return;
    }
    std::ofstream fto(to, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!fto.is_open()) {
        std::cerr << "Could not open output file" << std::endl;
        return;
    }

    b64dec(ffrom, fto);
}