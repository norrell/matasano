#include <string>
#include <vector>

#include "repeatkey_xor.h"
#include "typedefs.h"
#include "utils.h"

static std::string repeatkey_xor(const std::vector<byte> &plaintext, const std::vector<byte> &key)
{
    std::vector<byte> cipher(plaintext.size(), (byte) 0);

    for (unsigned int i = 0, j = 0; i < plaintext.size(); ++i, j = i % 3)
            cipher[i] = plaintext[i] ^ key[j];

    return bin_to_hex(cipher);
}

std::string repeatkey_xor(const std::string &plaintext, const std::string &key)
{
    std::vector<byte> v(plaintext.begin(), plaintext.end());
    std::vector<byte> k(key.begin(), key.end());
    return repeatkey_xor(v, k);
}