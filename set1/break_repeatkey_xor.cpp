#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <list>

#include "break_repeatkey_xor.h"
#include "singlebyte_xor.h"
#include "utils.h"

static size_t find_keylen(const byte *contents)
{
    double hmin = (double) INT_MAX;
    size_t keylenmin = 2;
    size_t keylenmax = 40;
    size_t bestkey;

    std::list<std::pair<size_t, double>> minkeylens;

    for (size_t try_keylen = keylenmin; try_keylen <= keylenmax; ++try_keylen) {
        std::vector<byte> first(contents, contents + try_keylen);
        std::vector<byte> second(contents + try_keylen, contents + 2 * try_keylen);
        std::vector<byte> third(contents + 2 * try_keylen, contents + 3 * try_keylen);
        std::vector<byte> fourth(contents + 3 * try_keylen, contents + 4 * try_keylen);

        double hnorm1 = ((double) hamming_distance(first, second) / (double) try_keylen);
        double hnorm2 = ((double) hamming_distance(second, third) / (double) try_keylen);
        double hnorm3 = ((double) hamming_distance(third, fourth) / (double) try_keylen);
        double hnorm4 = ((double) hamming_distance(first, third) / (double) try_keylen);
        double hnorm5 = ((double) hamming_distance(second, fourth) / (double) try_keylen);
        double hnorm6 = ((double) hamming_distance(first, fourth) / (double) try_keylen);

        double hnorm = (hnorm1 + hnorm2 + hnorm3 + hnorm4 + hnorm5 + hnorm6) / 6.0;

        if (hnorm < hmin) {
            hmin = hnorm;
            bestkey = try_keylen;
        }
    }
    return bestkey;
}

size_t read_file(byte **dynbuf, const std::string &filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
            return 0;
    }

    in.seekg(0, std::ios::end);
    size_t filelen = in.tellg();
    *dynbuf = new byte[filelen];
    in.seekg(0, std::ios::beg);
    in.read((char *) *dynbuf, filelen);
    in.close();

    return filelen;
}

std::string break_repeatkey_xor(const std::string &filename)
{
    byte *contents;
    size_t filelen = read_file(&contents, filename);

    size_t keylen = find_keylen(contents);
    size_t whole_blocks = filelen / keylen;
    size_t remaining_bytes = filelen % keylen;

    std::string keystr(keylen, '?');

    for (unsigned int ik = 0; ik < keylen; ++ik) {
        // THREADS ?
        std::vector<byte> block(whole_blocks + 1, 0);
        /*
         * 'block' should have room for all the necessary bytes until
         * the end of the file. If the file size is not divisible
         * by the 'keylen', the blocks for the first 'remaining_bytes'
         * of the key need one extra byte, otherwise not.
         */
        if (!(ik < remaining_bytes))
            block.resize(whole_blocks);

        for (unsigned int ib = 0, ic = ik; ib < block.size() && ic < filelen;
             ib++, ic += keylen)
            block[ib] = contents[ic];

        byte key = singlebyte_xor(block);
        keystr[(int) ik] = (char) key;
    }

    free(contents);
    return keystr;
}

void write_file(byte **dynbuf, const std::string &filename, size_t len) {
    std::ofstream out(filename, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
            return;
    }

    out.write((char *) *dynbuf, len);
    out.close();

    return;
}

void repeatkey_xor_decrypt_file(const std::string &cipherfile, const std::string &key)
{
    byte *contents;
    size_t filelen = read_file(&contents, cipherfile);

    /* XOR in-place */
    for (unsigned int i = 0, j = 0; i < filelen; ++i, j = i % key.size())
            contents[i] ^= key[j];

    std::string outfile = cipherfile;
    outfile += "_plain.txt";
    write_file(&contents, outfile, filelen);

    free(contents);

    return;
}